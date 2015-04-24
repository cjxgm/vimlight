#include "vim.hh"

namespace vimlight
{
	void vim::add(int i, hlrecord_type const& rec)
	{
		commands += "add(";
		commands += std::to_string(i);
		commands += ",\"";
		commands += rec.name;
		commands += "\",";
		commands += std::to_string(rec.y1);
		commands += ",";
		commands += std::to_string(rec.x1);
		commands += ",";
		commands += std::to_string(rec.x2 - rec.x1);
		commands += ")\n";
	}

	void vim::del(int i)
	{
		commands += "del(";
		commands += std::to_string(i);
		commands += ")\n";
	}
}

