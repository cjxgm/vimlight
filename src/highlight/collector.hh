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
			using highlight_list = highlight::list;
			using vim_type = vimlight::vim;

			void update(highlight_list hl, vim_type& vim);

		private:
			highlight_list highlights;
		};
	}
}

