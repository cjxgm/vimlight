#pragma once
#include "type.hh"
#include <unordered_map>

namespace vimlight
{
	namespace highlight
	{
		struct group
		{
			using table_type = std::unordered_map<clang_name_type, vim_name_type>;

			struct bad_file {};

			group() {}
			group(filename_type const& fn) { load(fn); }
			void load(filename_type const& fn);
			vim_name_type at(clang_name_type const& n) const { return table.at(n); }

		private:
			table_type table;
		};
	}
}

