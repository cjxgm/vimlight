#pragma once
#include "rule.hh"

namespace vimlight
{
	namespace analyzer_rules
	{
		// parameters in lambda's capture
		struct capture_parameter
		{
			static constexpr auto group() { return ":capture-parameter"; }

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

