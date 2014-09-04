#pragma once
// this module is for generating vim commands for highlighting.
#include "highlight/record.hh"
#include <string>
#include <vector>

namespace vimlight
{
	struct vim
	{
		using hlrecord_type = highlight::record;
		using name_type = hlrecord_type::name_type;
		using command_type = std::string;
		using commands_type = std::vector<command_type>;

		// generate a command to link a syntax group to highlight group
		// and add it to the commands buffer
		void link(name_type const& name);

		// generate a command to clear a syntax group
		// and add it to the commands buffer
		void clear(name_type const& name);

		// generate a command to add a syntax group
		// and add it to the commands buffer
		void add(hlrecord_type const& rec);

		// get the highlighting commands and clear the commands buffer
		commands_type get()
		{
			return std::move(commands);
		}

	private:
		commands_type commands;

		void push(command_type const& cmd);
	};
}

