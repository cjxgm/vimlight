#include "delta.hh"

namespace vimlight
{
	namespace highlight
	{
		void delta::update(list_type list, vim_type& vim)
		{
			auto size = std::min(old.size(), list.size());

			for (decltype(size) i=0; i<size; i++)
				if (old[i] != list[i]) {
					vim.rm (i);
					vim.add(i, list[i]);
				}
			for (auto i=size; i< old.size(); i++) vim.rm (i         );
			for (auto i=size; i<list.size(); i++) vim.add(i, list[i]);

			old = std::move(list);
		}
	}
}

