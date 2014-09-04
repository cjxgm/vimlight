#pragma once
#include "c.hh"
#include "internal.hh"

namespace clang
{
	struct index : public internal::guard<c::index::type>
	{
		using super_type = internal::guard<c::index::type>;

		index() : super_type(c::index::create(0, 0)) {}
		~index() override { if (owned) c::index::dispose(get()); }
	};
}

