#pragma once
#include "lua/all.hh"
#include "highlight/record.hh"
#include <string>




namespace vimlight
{
	struct vim
	{
		using command_func_type = lua::cfunction;
		using hlindex_type = unsigned;
		using hlrecord_type = highlight::record;
		using coord_type = hlrecord_type::coord_type;
		using  name_type = hlrecord_type:: name_type;
		using command_type = std::string;

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

	private:
		lua::state s;
		command_func_type cmd_func;

		void eval(const command_type& cmd);
		void region(hlindex_type idx,
				coord_type y1, coord_type x1,
				coord_type y2, coord_type x2);
		void link(hlindex_type idx, const name_type& name);
		void clear(hlindex_type idx);
	};
};

