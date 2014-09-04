#include "group.hh"
#include <fstream>
#include <sstream>




namespace vimlight
{
	namespace highlight
	{
		void group::load(const filename_type& fn)
		{
			std::ifstream f(fn);

			while (f) {
				std::string line;
				std::getline(f, line);
				std::istringstream ss(std::move(line));

				// skip blank line and line comment
				ss >> std::ws;
				if (ss.eof()) continue;
				if (ss.peek() == '#') continue;

				clang_name_type cname;
				  vim_name_type vname;
				if (!(ss >> cname >> vname))
					throw bad_file{};
				table[std::move(cname)] = std::move(vname);

				// skip trailing blanks and line-end comment
				ss >> std::ws;
				if (ss.eof()) continue;
				if (ss.peek() == '#') continue;

				// there are some unknown trailing characters
				throw bad_file{};
			}
		}
	}
}

