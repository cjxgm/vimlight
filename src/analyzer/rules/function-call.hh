#pragma once
#include "rule.hh"

namespace vimlight
{
	namespace analyzer_rules
	{
		// function call
		struct function_call
		{
			static constexpr auto group() { return ":function-call"; }

			static bool applicable(cursor_cref c)
			{
				if (c.kind != "call-expr") return false;

				auto r = c.reference();
				if (r.kind == "cxx-constructor") return false;
				if (r.kind == "invalid-file") return false;
				if (is_operator(c.name)) return false;

				auto oc = c.first_child();
				if (!oc) return false;

				auto& fc = oc.get();
				if (!is_one_of(fc.kind,
							"member-ref-expr",
							"unexposed-expr")) return false;

				auto fr = fc.reference();
				if (!is_one_of(fr.kind,
							"function-decl",
							"cxx-method",
							"cxx-destructor",
							"var-decl")) return false;

				return true;
			}

			static region apply(cursor_cref c)
			{
				auto oc = c.first_child();
				auto& fc = oc.get();
				return {{
					{ fc.pos.y, fc.pos.x, identifier_length(fc.name) }
				}};
			}
		};
	}
}

