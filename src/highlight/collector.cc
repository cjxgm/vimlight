#include "collector.hh"

namespace vimlight
{
	namespace highlight
	{
		void collector::update(highlight_list const& hl, vim_type& vim)
		{
			for (auto& rec: hl)
				if (!names[rec.name]) {
					vim.link(rec.name);
					names[rec.name] = true;
				}

			for (auto& name: names)
				vim.clear(name.first);

			for (auto& rec: hl)
				vim.add(rec);
		}
	}
}

