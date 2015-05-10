#pragma once
#include "rule.hh"

namespace vimlight
{
	namespace analyzer_rules
	{
		// function declaration
		struct function_decl
		{
			static constexpr auto group() { return ":function-decl"; }

			static bool applicable(cursor_cref c)
			{
				if (!is_one_of(c.kind,
							"function-decl",
							"function-template",
							"cxx-method",
							"cxx-constructor",
							"cxx-destructor")) return false;

				if (is_operator(c.name)) return false;

				return true;
			}

			static region apply(cursor_cref c)
			{
				return {{
					{ c.pos.y, c.pos.x, identifier_length(c.name) }
				}};
			}
		};
	}
}

