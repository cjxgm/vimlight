#include "vim.hh"

namespace vimlight
{
	namespace vim
	{
		commands_type highlight(highlight_list const& hls)
		{
			commands_type cmds{"clear()\n"};

			for (auto& rec: hls) {
				cmds += "add(\"";
				cmds += rec.name;
				cmds += "\",";
				cmds += std::to_string(rec.y1);
				cmds += ",";
				cmds += std::to_string(rec.x1);
				cmds += ",";
				cmds += std::to_string(rec.x2 - rec.x1);
				cmds += ")\n";
			}

			log << "[vimlight commands]\n" << cmds;
			return std::move(cmds);
		}
	}
}

