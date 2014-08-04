#include "vim.hh"
#include <sstream>




namespace vimlight
{
	void vim::push(const command_type& cmd)
	{
		commands.push_back(cmd);
	}

	void vim::region(hlindex_type idx,
			coord_type y1, coord_type x1,
			coord_type y2, coord_type x2)
	{
		std::stringstream ss;
		ss	<< "syn region vimlight" << idx
			<< " start=+\\%" << y1 << "l\\%" << x1 << "c+"
			<<   " end=+\\%" << y2 << "l\\%" << x2 << "c+";
		push(ss.str());
	}

	void vim::link(hlindex_type idx, const name_type& name)
	{
		std::stringstream ss;
		ss << "hi link vimlight" << idx << " " << name;
		push(ss.str());
	}

	void vim::clear(hlindex_type idx)
	{
		std::stringstream ss;
		ss << "syn clear vimlight" << idx;
		push(ss.str());
	}
};

