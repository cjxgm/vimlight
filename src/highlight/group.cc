#include "group.hh"
#include <fstream>




namespace vimlight
{
	namespace highlight
	{
		void group::load(const filename_type& fn)
		{
			std::ifstream f(fn);

			clang_name_type cname;
			  vim_name_type vname;

			while (f >> cname >> vname)
				table[cname] = vname;
		}
	}
};

