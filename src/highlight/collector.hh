#pragma once
// collects highlights and call vim for highlighting
#include "list.hh"
#include "../vim.hh"
#include <unordered_map>

namespace vimlight
{
	namespace highlight
	{
		struct collector
		{
			using hlrecord_type = highlight::record;
			using name_type = hlrecord_type::name_type;
			using name_list = std::unordered_map<name_type, bool>;
			using highlight_list = highlight::list;
			using vim_type = vimlight::vim;

			void update(highlight_list const& hl, vim_type& vim);

		private:
			name_list names;
		};
	}
}

