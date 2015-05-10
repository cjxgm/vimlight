#pragma once
#include "rule.hh"

namespace vimlight
{
	namespace analyzer_rules
	{
		// parameters
		struct parameter
		{
			static constexpr auto group() { return ":parameter"; }

			static bool applicable(cursor_cref c)
			{
				return (c.kind == "decl-ref-expr" && c.reference().kind == "parm-decl");
			}

			static region apply(cursor_cref c)
			{
				return {{
					{ c.head.y, c.head.x, c.tail.x-c.head.x },
				}};
			}
		};
	}
}

