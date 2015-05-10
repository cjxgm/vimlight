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

			group() {}
			group(filename_type const& fn) { load(fn); }
			void load(filename_type const& fn);
			vim_name_type at(clang_name_type const& n) const { return table.at(n); }
			auto find(clang_name_type const& n) const { return table.find(n); }

		private:
			table_type table;

			friend auto end(group const& g) { return g.table.end(); }
		};
	}
}

