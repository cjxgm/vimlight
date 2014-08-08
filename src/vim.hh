#pragma once
// this module is for generating vim commands for highlighting.
#include "highlight/record.hh"
#include <string>
#include <vector>

namespace vimlight
{
	struct vim
	{
		using hlindex_type = unsigned;
		using hlrecord_type = highlight::record;
		using coord_type = hlrecord_type::coord_type;
		using  name_type = hlrecord_type:: name_type;
		using command_type = std::string;
		using commands_type = std::vector<command_type>;

		// generate a command to add a highlighting region
		// and add it to the commands buffer
		void add(hlindex_type idx, hlrecord_type rec)
		{
			region(idx, rec.y1, rec.x1, rec.y2, rec.x2);
			link(idx, rec.name);
		}

		// generate a command to remove a highlighting region
		// and add it to the commands buffer
		void rm(hlindex_type idx)
		{
			clear(idx);
		}

		// get the highlighting commands and clear the commands buffer
		commands_type get()
		{
			return std::move(commands);
		}

	private:
		commands_type commands;

		void push(const command_type& cmd);
		void region(hlindex_type idx,
				coord_type y1, coord_type x1,
				coord_type y2, coord_type x2);
		void link(hlindex_type idx, const name_type& name);
		void clear(hlindex_type idx);
	};
};

