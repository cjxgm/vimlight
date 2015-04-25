#pragma once
#include "c.hh"
#include "resource.hh"
#include <vector>

namespace clang
{
	struct diagnostic;

	struct diagnostic_set : public resource::unique<c::diagnostic::set::type>
	{
		using self_type = diagnostic_set;
		using super_type = unique;
		using diagnostics_type = std::vector<diagnostic>;

		diagnostic_set(value_type value)
			: super_type(value, c::diagnostic::set::dispose) {}

		diagnostics_type all();
	};
}

