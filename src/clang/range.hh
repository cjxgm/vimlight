#pragma once
#include "c.hh"
#include "internal.hh"
#include "location.hh"
#include <utility>

namespace clang
{
	struct range : public internal::bin<c::range::type>
	{
		using super_type = internal::bin<c::range::type>;

		range(value_type value) : super_type(value) {}

		location head() const { return c::range::get_start(get()); }
		location tail() const { return c::range::get_end  (get()); }
	};
};

