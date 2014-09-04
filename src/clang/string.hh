#pragma once
#include "c.hh"
#include "internal.hh"
#include <string>

namespace clang
{
	struct string : public internal::guard<c::string::type>
	{
		using super_type = internal::guard<c::string::type>;
		using cstring_type = char const*;
		using  string_type = std::string;

		string(value_type value) : super_type(value) {}
		~string() override { if (owned) c::string::dispose(get()); }

		operator cstring_type() const { return c::string::cstr(get()); }
		operator  string_type() const { return operator cstring_type(); }
	};
}

