#include "analyzer.hh"
#include "log.hh"
#include <utility>

namespace vimlight
{
	auto analyzer::parse(
			source_type const& src,
			 group_type const& group) -> list_type
	{
		list_type list;
		tu.parse(src);

		auto diags = tu.diagnostics();
		log << "analyzer::parse():\n";
		for (auto& diag: diags) {
			log << "\t[error]\n"
				<< "\t\t" << diag.text() << '\n';

			auto loc = diag.location();
			if (!loc.is_from_main()) continue;

			auto pos =  loc.position();
			list.push_back({
					pos.y, pos.x,
					pos.y, pos.x+1,
					"error" });

			log << "\t\t" << pos.y << ", " << pos.x << '\n';
		}

		tu.cursor().each_child([&](clang::cursor const& cursor) {
			auto range = cursor.range();
			auto head = range.head();
			if (!head.is_from_main())
				return false;

			auto head_pos =       head  .position();
			auto tail_pos = range.tail().position();
			auto kind = cursor.kind().name();
			auto name = cursor.name();
			log << "\t[" << kind << "]\n"
				<< "\t\t\"" << name << "\"\n"
				<< "\t\t" << head_pos.y << ", " << head_pos.x
				<< " -> " << tail_pos.y << ", " << tail_pos.x << '\n';
			log << "\t\t{" << head.file() << "}\n";

			auto ref = cursor.reference();
			auto ref_kind = ref.kind().name();
			log << "\t\t::[" << ref_kind << "]\n"
				<< "\t\t\t\"" << ref.name() << "\"\n"
				<< "\t\t\t{" << ref.range().head().file() << "}\n";

			try {
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
					log << "\t\t" << kind << '\n';
				}

				else if (kind == "DeclRefExpr" && ref_kind == "FunctionDecl" &&
						name.size() < tail_pos.x-head_pos.x) {
					auto kind = group.at("template_function_call");
					list.push_back({
							head_pos.y, head_pos.x,
							head_pos.y, head_pos.x+int(name.size()),
							kind });
					log << "\t\t" << kind << '\n';
				}

				else if (kind == "DeclRefExpr" && ref_kind == "FunctionDecl") {
					// skip operator
					if (name.size() > tail_pos.x-head_pos.x)
						return true;
					// FIXME: not work for "std::move(...)"
					auto kind = group.at("function_call");
					list.push_back({
							head_pos.y, head_pos.x,
							head_pos.y, head_pos.x+int(name.size()),
							kind });
					log << "\t\t" << kind << '\n';
				}

				else if (kind == "MemberRefExpr" && ref_kind == "CXXMethod") {
					auto kind = group.at("member_call");
					list.push_back({
							tail_pos.y, tail_pos.x-int(name.size()),
							tail_pos.y, tail_pos.x,
							kind });
					log << "\t\t" << kind << '\n';
				}

				else if (kind == "DeclRefExpr" && ref_kind == "ParmDecl") {
					auto kind = group.at("parameter");
					list.push_back({
							head_pos.y, head_pos.x,
							tail_pos.y, tail_pos.x,
							kind });
					log << "\t\t" << kind << '\n';
				}

				else if (kind == "MemberRefExpr" && ref_kind == "FieldDecl") {
					auto kind = group.at("member");
					list.push_back({
							tail_pos.y, tail_pos.x-int(name.size()),
							tail_pos.y, tail_pos.x,
							kind });
					log << "\t\t" << kind << '\n';
				}

				else {
					list.push_back({
							head_pos.y, head_pos.x,
							tail_pos.y, tail_pos.x,
							group.at(kind) });
					log << "\t\t" << group.at(kind) << '\n';
				}
				return true;
			}
			catch (std::out_of_range) {}

			return true;
		});

		return std::move(list);
	}
}

