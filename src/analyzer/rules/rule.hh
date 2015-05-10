#pragma once
#include "utils.hh"
#include "../../clang/cursor.hh"
#include "../../highlight/region.hh"
#include <array>

namespace vimlight
{
	namespace analyzer_rules
	{
		using namespace analyzer_utils;
		using clang::cursor;
		using cursor_cref = cursor const&;

		template <std::size_t N>
		using regions = std::array<highlight::region, N>;
		using region = regions<1>;
	}
}

