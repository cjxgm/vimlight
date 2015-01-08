#pragma once
#include "c.hh"
#include "resource.hh"
#include "location.hh"
#include "string.hh"
#include <utility>
#include <string>

namespace clang
{
	struct diagnostic : public resource::unique<c::diagnostic::type>
	{
		using self_type = diagnostic;
		using super_type = unique;
		using text_type = std::string;

		diagnostic(value_type value)
			: super_type(value, c::diagnostic::dispose) {}

		location location() { return c::diagnostic::get_location(get()); }
		text_type text()
		{
			return clang::string{
				c::diagnostic::format(get(),
						c::options::default_diagnostic_display())};
		}
	};
}

