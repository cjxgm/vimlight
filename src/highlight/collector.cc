#include "collector.hh"

namespace vimlight
{
	namespace highlight
	{
		void collector::update(highlight_list hl, vim_type& vim)
		{
			for (auto i = 0u; i < highlights.size(); i++)
				vim.del(i);

			auto i = 0u;
			for (auto& rec: hl)
				vim.add(i++, rec);
			highlights = std::move(hl);
		}
	}
}

