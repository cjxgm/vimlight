#pragma once
#include "list.hh"
#include "../vim.hh"

namespace vimlight
{
	namespace highlight
	{
		struct delta
		{
			using list_type = highlight::list;
			using  vim_type = vimlight::vim;

			void update(list_type list, vim_type& vim);

		private:
			list_type old;
		};
	}
}

