#include "diagnostic_set.hh"
#include "diagnostic.hh"
#include <utility>	// for std::move

namespace clang
{
	auto diagnostic_set::all() -> diagnostics_type
	{
		auto size = c::diagnostic::set::get_count(get());

		diagnostics_type diags;
		diags.reserve(size);

		for (auto i=0u; i<size; i++)
			diags.emplace_back(c::diagnostic::set::get(get(), i));

		return std::move(diags);
	}
}

