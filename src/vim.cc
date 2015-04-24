#include "vim.hh"
#include "log.hh"
#include <sstream>




namespace vimlight
{
	void vim::push(command_type const& cmd)
	{
		log << "vim command: "<< cmd << "\n";
		commands.push_back(cmd);
	}

	void vim::add(int i, hlrecord_type const& rec)
	{
		std::ostringstream ss;
		ss	<< "add(" << i << ", \"" << rec.name << "\", "
			<< rec.y1 << ", " << rec.x1 << ", "
			<< rec.x2-rec.x1 << ")";
		// TODO
//		ss	<< "call matchaddpos(\"vimlight_" << rec.name
//			<< "\", [[" << rec.y1 << ", " << rec.x1
//			<< ", " << rec.x2-rec.x1 << "]])";
		push(ss.str());
	}

	void vim::del(int i)
	{
		std::ostringstream ss;
		ss	<< "del(" << i << ")";
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

