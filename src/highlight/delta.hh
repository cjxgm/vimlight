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

			void update(list_type list, vim_type& vim)
			{
				auto size = std::min(old.size(), list.size());

				for (int i=0; i<size; i++)
					if (old[i] != list[i]) {
						vim.rm (i);
						vim.add(i, list[i]);
					}
				for (int i=size; i< old.size(); i++) vim.rm (i         );
				for (int i=size; i<list.size(); i++) vim.add(i, list[i]);

				old = std::move(list);
			}

		private:
			list_type old;
		};
	};
};

