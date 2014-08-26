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

		auto diags = tu.diagnostics();
		log << "analyzer::parse():" << endl;
		for (auto& diag: diags) {
			log << "\t[error]" << endl
				<< "\t\t" << diag.text() << endl;

			auto loc = diag.location();
			if (!loc.is_from_main()) continue;

			auto pos =  loc.position();
			list.push_back({
					pos.y, pos.x,
					pos.y, pos.x+1,
					"error" });

			log << "\t\t" << pos.y << ", " << pos.x << endl;
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
				<< " -> " << tail_pos.y << ", " << tail_pos.x << endl;

			try {
				list.push_back({
						head_pos.y, head_pos.x,
						tail_pos.y, tail_pos.x,
						group.at(kind)});
				log << "\t\t" << group.at(kind) << endl;
				return true;
			}
			catch (std::out_of_range) {}

			return true;
		});

		return std::move(list);
	}
};

