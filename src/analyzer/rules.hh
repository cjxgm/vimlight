#pragma once
#include <tuple>
#include "rules/init-list-brace.hh"
#include "rules/function-call.hh"

namespace vimlight
{
	using all_analyzer_rules = std::tuple<
		analyzer_rules::init_list_brace,
		analyzer_rules::function_call
	>;
}

