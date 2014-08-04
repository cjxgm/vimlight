#pragma once
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

		vim(command_func_type f) : cmd_func(f) {}

		void add(hlindex_type idx, hlrecord_type rec)
		{
			region(idx, rec.y1, rec.x1, rec.y2, rec.x2);
			link(idx, rec.name);
		}

		void rm(hlindex_type idx)
		{
			clear(idx);
		}

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

