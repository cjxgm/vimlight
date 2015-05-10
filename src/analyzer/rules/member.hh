#pragma once
#include "rule.hh"

namespace vimlight
{
	namespace analyzer_rules
	{
		// member or field
		struct member
		{
			static constexpr auto group() { return ":member"; }

			static bool applicable(cursor_cref c)
			{
				return (c.kind == "member-ref-expr" && c.reference().kind == "field-decl");
			}

			static region apply(cursor_cref c)
			{
				auto idlen = identifier_length(c.name);
				return {{
					{ c.tail.y, c.tail.x-idlen, idlen },
				}};
			}
		};
	}
}

