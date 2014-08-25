#include "analyzer.hh"
#include "log.hh"
#include <utility>

namespace vimlight
{
	auto analyzer::parse(
			const source_type& src,
			const  group_type& group) -> list_type
	{
		list_type list;
		tu.parse(src);

		log << "analyzer::parse():" << endl;

		auto diags = tu.diagnostics();
		for (auto& diag: diags) {
			auto pos = diag.location().position();
			list.push_back({
					pos.y, pos.x,
					pos.y, pos.x+1,
					"error" });
			log << "\t[error]  " << pos.y << ", " << pos.x << endl;
		}

		tu.cursor().each_child([&](const clang::cursor& cursor) {
			auto range = cursor.range();
			auto head = range.head();
			if (!head.is_from_main())
				return false;

			auto head_pos =       head  .position();
			auto tail_pos = range.tail().position();
			auto kind = cursor.kind().name();
			log << "\t[" << kind << "]" << endl
				<< "\t\t\"" << cursor.name() << "\"" << endl
				<< "\t\t" << head_pos.y << ", " << head_pos.x
				<< " -> " << tail_pos.y << ", " << tail_pos.x << "\t";

			try {
				list.push_back({
						head_pos.y, head_pos.x,
						tail_pos.y, tail_pos.x,
						group.at(kind)});
				log << "yes" << endl;
				return true;
			}
			catch (std::out_of_range) {}
			log << "no" << endl;

			return true;
		});

		return std::move(list);
	}
};

