#pragma once
// this module is for generating vim commands for highlighting.
#include "highlight/record.hh"
#include "log.hh"
#include <string>

namespace vimlight
{
	struct vim
	{
		using hlrecord_type = highlight::record;
		using commands_type = std::string;

		// generate a command to add a highlight
		// and add it to the commands buffer
		void add(int i, hlrecord_type const& rec);

		// generate a command to delete a highlight
		// and add it to the commands buffer
		void del(int i);

		// get the highlighting commands and clear the commands buffer
		commands_type get()
		{
			log << "[vimlight commands]\n" << commands;
			return std::move(commands);
		}

	private:
		commands_type commands;
	};
}

