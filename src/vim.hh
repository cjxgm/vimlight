#pragma once
// this module is for generating vim commands for highlighting.
#include "highlight/type.hh"
#include "highlight/region.hh"
#include "log.hh"
#include <string>
#include <utility>		// for std::move

namespace vimlight
{
	struct vim
	{
		using     region = highlight::region;
		using group_name = highlight::vim_name_type;
		using     region_cref =     region const&;
		using group_name_cref = group_name const&;
		using commands = std::string;

		void clear();
		void highlight(group_name_cref g, region_cref r);

		auto get()
		{
			log << "[vimlight-command]\n" << cmds;
			return std::move(cmds);
		}

	private:
		commands cmds;
	};
}

