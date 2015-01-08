#pragma once
#include "c.hh"
#include "resource.hh"

namespace clang
{
	struct index : public resource::unique<c::index::type>
	{
		using super_type = unique;

		index() : super_type(c::index::create(0, 0), c::index::dispose) {}
	};
}

