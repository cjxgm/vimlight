#pragma once
// this module is for generating vim commands for highlighting.
#include "highlight/list.hh"
#include "log.hh"
#include <string>

namespace vimlight
{
	namespace vim
	{
		using highlight_list = highlight::list;
		using commands_type = std::string;

		commands_type highlight(highlight_list const& hls);
	}
}

