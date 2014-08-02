#include "analyzer.hh"
#include <utility>

namespace vimlight
{
	auto analyzer::parse(
			const source_type& src,
			const  group_type& group) -> list_type
	{
		list_type list;
		tu.parse(src);

		tu.cursor().each_child([&](const clang::cursor& cursor) {
			auto range = cursor.range();
			auto head = range.head();
			if (!head.is_from_main())
				return false;

			auto head_pos =       head  .position();
			auto tail_pos = range.tail().position();
			auto kind = cursor.kind().name();

			try {
				list.push_back({
						head_pos.y, head_pos.x,
						tail_pos.y, tail_pos.x,
						group.at(kind)});
				return true;
			}
			catch (std::out_of_range) {}

			return true;
		});

		return std::move(list);
	}
};

