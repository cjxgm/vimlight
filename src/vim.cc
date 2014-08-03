#include "vim.hh"
#include <sstream>




namespace vimlight
{
	void vim::eval(const command_type& cmd)
	{
		s.call(0, cmd_func, cmd);
	}

	void vim::region(hlindex_type idx,
			coord_type y1, coord_type x1,
			coord_type y2, coord_type x2)
	{
		std::stringstream ss;
		ss	<< "syn region vimlight" << idx
			<< " start=+\\%" << y1 << "l\\%" << x1 << "c+"
			<<   " end=+\\%" << y2 << "l\\%" << x2 << "c+";
		eval(ss.str());
	}

	void vim::link(hlindex_type idx, const name_type& name)
	{
		std::stringstream ss;
		ss << "hi link vimlight" << idx << " " << name;
		eval(ss.str());
	}

	void vim::clear(hlindex_type idx)
	{
		std::stringstream ss;
		ss << "syn clear vimlight" << idx;
		eval(ss.str());
	}
};

