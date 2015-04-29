#pragma once
#include "c.hh"
#include "resource.hh"
#include "location.hh"
#include <utility>

namespace clang
{
	struct range : resource::bin<c::range::type>
	{
		using super_type = bin;

		range(value_type value) : super_type(value) {}

		location head() const { return c::range::get_start(get()); }
		location tail() const { return c::range::get_end  (get()); }
	};
}

