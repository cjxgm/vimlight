#include "vim.hh"

namespace vimlight
{
	void vim::clear()
	{
		cmds += "clear()\n";
	}

	void vim::highlight(group_name_cref g, region_cref r)
	{
		cmds += "add(\"";
		cmds += g;
		cmds += "\",";
		cmds += std::to_string(r.y);
		cmds += ",";
		cmds += std::to_string(r.x);
		cmds += ",";
		cmds += std::to_string(r.w);
		cmds += ")\n";
	}
}

