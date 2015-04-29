#pragma once
#include <tuple>
#include "rules/init-list-brace.hh"

namespace vimlight
{
	using all_analyzer_rules = std::tuple<
		analyzer_rules::init_list_brace
	>;
}

