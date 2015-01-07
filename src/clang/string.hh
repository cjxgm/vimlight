#pragma once
#include "c.hh"
#include "resource.hh"
#include <string>

namespace clang
{
	struct string : public resource::unique<c::string::type>
	{
		using super_type = unique;
		using cstring_type = char const*;
		using  string_type = std::string;

		string(value_type value) : super_type(value, c::string::dispose) {}

		operator cstring_type() const { return c::string::cstr(get()); }
		operator  string_type() const { return operator cstring_type(); }
	};
}

