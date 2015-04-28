#include "analyzer.hh"
#include "log.hh"
#include <utility>
#include <string>
#include <algorithm>

namespace
{
	bool is_identifier(char c)
	{
		return (('a' <= c && c <= 'z') ||
				('A' <= c && c <= 'Z') ||
				('0' <= c && c <= '9') ||
				c == '_' || c == '$');
	}

	int identifier_length(std::string const& x)
	{
		if (x.empty()) return 0;
		auto first = begin(x);
		if (*first == '~') ++first;
		auto it = std::find_if_not(first, end(x), is_identifier);
		return it - begin(x);
	}

	bool is_operator(std::string const& x)
	{
		constexpr auto op = "operator";
		constexpr auto op_size = sizeof(op);
		static_assert(op_size == 8, "portability issue?");
		if (x.size() <= op_size) return false;
		if (x.substr(0, op_size) != op) return false;
		return !is_identifier(x[op_size]);
	}
}


namespace vimlight
{
	auto analyzer::parse(
			source_type const& src,
			 group_type const& group) -> list_type
	{
		list_type list;
		tu.parse(src);

		// analyze errors
		auto diags = tu.diagnostics();
		log << "analyzer::parse():\n";
		for (auto& diag: diags)
			try {
				log << "\t[error]\n"
					<< "\t\t" << diag.text() << '\n';

				auto loc = diag.location();
				if (!loc.is_from_main()) continue;

				auto pos =  loc.position();
				list.push_back({ pos.y, pos.x, pos.y, pos.x+1, group.at("error") });
				log << "\t\t" << pos.y << ", " << pos.x << '\n';

				auto ranges = diag.ranges();
				for (auto& range: ranges) {
					auto head = range.head();
					if (!head.is_from_main()) continue;

					auto head_pos =       head  .position();
					auto tail_pos = range.tail().position();

					list.push_back({ head_pos.y, head_pos.x,
							tail_pos.y, tail_pos.x+1, group.at("error_range") });
					log << "\t\t" << head_pos.y << ", " << head_pos.x
						<< " -> " << tail_pos.y << ", " << tail_pos.x
						<< "\n";
				}
			}
			catch (std::out_of_range) {}

		// semantic highlighting
		tu.cursor().each_child([&](clang::cursor const& cursor) {
			auto range = cursor.range();
			auto head = range.head();
			if (!head.is_from_main())
				return false;

			auto head_pos =       head  .position();
			auto tail_pos = range.tail().position();
			auto pos = cursor.location().position();
			auto kind = cursor.kind().name();
			auto name = cursor.name();
			log << "\t[" << kind << "]\n"
				<< "\t\t\"" << name << "\"\n"
				<< "\t\t" << head_pos.y << ", " << head_pos.x
				<< " -> " << tail_pos.y << ", " << tail_pos.x
				<< " @ " << pos.y << ", " << pos.x << '\n';
			log << "\t\t{" << head.file() << "}\n";

			auto ref = cursor.reference();
			auto ref_kind = ref.kind().name();
			log << "\t\t::[" << ref_kind << "]\n"
				<< "\t\t\t\"" << ref.name() << "\"\n"
				<< "\t\t\t{" << ref.range().head().file() << "}\n";

			try {
				// braces of trivial initializer list
				if (kind == "InitListExpr") {
					kind = group.at("init_list_brace");
					log << "\t[init_list_brace]\n"
						<< "\t\t" << head_pos.y << ", " << head_pos.x
						<< " -> " << head_pos.y << ", " << head_pos.x+1
						<< '\n'
						<< "\t\t" << tail_pos.y << ", " << tail_pos.x-1
						<< " -> " << tail_pos.y << ", " << tail_pos.x
						<< '\n';
					list.push_back({
							head_pos.y, head_pos.x,
							head_pos.y, head_pos.x+1,
							kind });
					list.push_back({
							tail_pos.y, tail_pos.x-1,
							tail_pos.y, tail_pos.x,
							kind });
					log << "\t\t" << kind << " (initializer list braces)\n";
				}

				// function call
				else if (kind == "CallExpr" && ref_kind != "CXXConstructor" && !is_operator(name)) {
					auto oc = cursor.first_child();
					if (oc) {
						auto fc = oc.get();
						auto fc_kind = fc.kind().name();
						auto fc_ref_kind = fc.reference().kind().name();
						if ((fc_kind == "MemberRefExpr" ||
									fc_kind == "UnexposedExpr") &&
								(fc_ref_kind == "FunctionDecl" ||
									fc_ref_kind == "CXXMethod" ||
									fc_ref_kind == "CXXDestructor" ||
									fc_ref_kind == "VarDecl")) {
							auto kind = group.at("function_call");
							auto pos = fc.location().position();
							int name_size = identifier_length(fc.name());
							list.push_back({ pos.y, pos.x, pos.y, pos.x+name_size, kind });
							log << "\t\t" << kind << " (function call)\n";
						}
					}
				}

				// function declaration
				else if (kind == "FunctionDecl" ||
						kind == "FunctionTemplate" ||
						kind == "CXXMethod" ||
						kind == "CXXConstructor" ||
						kind == "CXXDestructor") {
					if (!is_operator(name)) {
						auto kind = group.at("function_decl");
						int name_size = identifier_length(name);
						list.push_back({ pos.y, pos.x, pos.y, pos.x+name_size, kind });
						log << "\t\t" << kind << " (function declaration)\n";
					}
				}

				// parameters in lambdas capture
				else if (kind == "DeclRefExpr" && ref_kind == "ParmDecl") {
					auto kind = group.at("parameter");
					list.push_back({
							head_pos.y, head_pos.x,
							tail_pos.y, tail_pos.x,
							kind });
					log << "\t\t" << kind << " (parameter)\n";
				}

				// member/field
				else if (kind == "MemberRefExpr" && ref_kind == "FieldDecl") {
					auto kind = group.at("member");
					list.push_back({
							tail_pos.y, tail_pos.x-int(name.size()),
							tail_pos.y, tail_pos.x,
							kind });
					log << "\t\t" << kind << " (member)\n";
				}

				// other range
				else {
					list.push_back({
							head_pos.y, head_pos.x,
							tail_pos.y, tail_pos.x,
							group.at(kind) });
					log << "\t\t" << group.at(kind) << " (" << kind << ")\n";
				}
				return true;
			}
			catch (std::out_of_range) {}

			return true;
		});

		return std::move(list);
	}
}

