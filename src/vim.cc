#include "vim.hh"
#include <sstream>




namespace vimlight
{
	void vim::push(command_type const& cmd)
	{
		commands.push_back(cmd);
	}

	void vim::add(hlrecord_type const& rec)
	{
		std::ostringstream ss;
		ss	<< "syn match vimlight_" << rec.name
			<< " +\\%" << rec.y1 << "l\\%" << rec.x1 << "c"
			<< ".*\\%" << rec.y2 << "l\\%" << rec.x2 << "c+";
		push(ss.str());
	}

	void vim::link(name_type const& name)
	{
		std::ostringstream ss;
		ss << "hi link vimlight_" << name << " " << name;
		push(ss.str());
	}

	void vim::clear(name_type const& name)
	{
		std::ostringstream ss;
		ss << "syn clear vimlight_" << name;
		push(ss.str());
	}
}

