#pragma once
#include <tuple>
#include "rules/init-list-brace.hh"
#include "rules/function-call.hh"
#include "rules/function-decl.hh"
#include "rules/parameter.hh"
#include "rules/member.hh"

namespace vimlight
{
	using all_analyzer_rules = std::tuple<
		analyzer_rules::init_list_brace,
		analyzer_rules::function_call,
		analyzer_rules::function_decl,
		analyzer_rules::parameter,
		analyzer_rules::member
	>;
}

