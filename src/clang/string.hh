#pragma once
#include "c.hh"
#include "resource.hh"
#include <string>

namespace clang
{
	struct string : resource::unique<c::string::type>
	{
		using super_type = unique;
		using string_type = std::string;

		string(value_type value) : super_type(value, c::string::dispose) {}

		operator  string_type() const { return c::string::cstr(get()); }
	};
}

