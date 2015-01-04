#include "analyzer.hh"
#include "log.hh"
#include <utility>

static bool is_identifier(char c)
{
	return (('a' <= c && c <= 'z') ||
			('A' <= c && c <= 'Z') ||
			('0' <= c && c <= '9') ||
			c == '_' || c == '$');
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
		for (auto& diag: diags) {
			log << "\t[error]\n"
				<< "\t\t" << diag.text() << '\n';

			auto loc = diag.location();
			if (!loc.is_from_main()) continue;

			auto pos =  loc.position();
			list.push_back({ pos.y, pos.x, pos.y, pos.x+1, "error" });

			log << "\t\t" << pos.y << ", " << pos.x << '\n';
		}

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

				// special function/static member call
				else if (kind == "DeclRefExpr" && (
							ref_kind == "FunctionDecl" ||
							ref_kind == "CXXMethod")) {
					int extent = tail_pos.x - head_pos.x;
					int name_size = name.size();
					static const std::string op{"operator"};
					int op_size = op.size();
					bool is_operator = (
							name_size > op_size &&
							name.substr(0, op_size) == op &&
							!is_identifier(name[op_size]));

					/* call operator function implicitly

						000000000011111111
						012345678901234567
						x["abcdeasdasd"];

						is_operator: true
						name: operator[]
						name_size: 10
						op_size: 8
						real_size: 10 - 8 = 2
						pos: 01
						head_pos: 01
						tail_pos: 17
						extent: 17 - 01 =16
					*/
					if (is_operator) {
						auto kind = group.at("operator_call");
						int real_size = name_size - op_size;
						if (real_size == 2) {
							list.push_back({
									head_pos.y, head_pos.x,
									head_pos.y, head_pos.x+1,
									kind });
							list.push_back({
									tail_pos.y, tail_pos.x-1,
									tail_pos.y, tail_pos.x,
									kind });
						}
						else list.push_back({
								head_pos.y, head_pos.x,
								tail_pos.y, tail_pos.x,
								kind });
						log << "\t\t" << kind << " (operator call)\n";
					}

					/* surrounded function call

						a function name may be surrounded by:
						1.	namespace/struct name then "::"
						3.	template arguments with "<>" around them

						0000000000111111111122222
						0123456789012345678901234
						foo::bar<baz>(123, 456);
						`----+-------+--- head_pos
						     |       `--- tail_pos
						`----+-------'--- extent
						     `----------- pos

						is_operator: false
						name: bar
						name_size: 3
						pos: 05
						head_pos: 00
						tail_pos: 13
						extent: 13 - 00 = 13
					*/
					else if (name_size < extent) {
						auto kind = group.at("special_function_call");
						list.push_back({ pos.y, pos.x, pos.y, pos.x+name_size, kind });
						log << "\t\t" << kind << " (special function call)\n";
					}
				}

				// special member call
				else if (kind == "MemberRefExpr" && ref_kind == "CXXMethod") {
					int name_size = name.size();

					/* surrounded member call

						a function name may be surrounded by:
						1.	object name then "."
						3.	template arguments with "<>" around them

						000000000011111111112222
						012345678901234567890123
						foo.bar<baz>(123, 456);
						`---+-------+--- head_pos
						    |       `--- tail_pos
						`---+-------'--- extent
						    `----------- pos

						name: bar
						name_size: 3
						pos: 04
						head_pos: 00
						tail_pos: 12
						extent: 12 - 00 = 12
					*/
					// FIXME: multi-line not supported.
					if (pos.x + name_size < tail_pos.x) {
						auto kind = group.at("special_member_call");
						list.push_back({ pos.y, pos.x, pos.y, pos.x+name_size, kind });
						log << "\t\t" << kind << " (special member call)\n";
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

