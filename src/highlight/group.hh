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
			group(const filename_type& fn) { load(fn); }
			void load(const filename_type& fn);
			vim_name_type at(const clang_name_type& n) const { return table.at(n); }

		private:
			table_type table;
		};
	}
}

