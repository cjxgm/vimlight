#pragma once
#include "rule.hh"

namespace vimlight
{
	namespace analyzer_rules
	{
		// braces of trivial initializer list
		struct init_list_brace
		{
			static constexpr auto group() { return "init-list-brace"; }

			static bool applicable(cursor_cref c)
			{
				return (c.kind == "init-list-expr");
			}

			static regions<2> apply(cursor_cref c)
			{
				return {{
					{ c.head.y, c.head.x  , 1 },
					{ c.tail.y, c.tail.x-1, 1 },
				}};
			}
		};
	}
}

