#pragma once
#include "c.hh"
#include "resource.hh"
#include "location.hh"
#include "range.hh"
#include "string.hh"
#include "diagnostic_set.hh"
#include <utility>
#include <string>
#include <vector>

namespace clang
{
	struct diagnostic : public resource::unique<c::diagnostic::type>
	{
		using self_type = diagnostic;
		using super_type = unique;
		using text_type = std::string;
		using ranges_type = std::vector<range>;
		using diagnostics_type = std::vector<self_type>;

		diagnostic(value_type value)
			: super_type(value, c::diagnostic::dispose) {}

		location location() { return c::diagnostic::get_location(get()); }

		ranges_type ranges()
		{
			auto size = c::diagnostic::get_range_count(get());
			ranges_type rs;
			rs.reserve(size);
			for (auto i=0u; i<size; i++)
				rs.emplace_back(c::diagnostic::get_range(get(), i));
			return rs;
		}

		diagnostics_type childs()
		{
			diagnostic_set diags{c::diagnostic::get_childs(get())};
			return diags.all();
		}

		text_type text()
		{
			return clang::string{
				c::diagnostic::format(get(),
						c::options::default_diagnostic_display())};
		}
	};
}

