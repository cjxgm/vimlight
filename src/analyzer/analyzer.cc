#include "analyzer.hh"
#include "../log.hh"
#include "rules.hh"

namespace vimlight
{
	namespace
	{
		using  group_cref = analyzer::group_cref;
		using     vim_ref = analyzer::vim_ref;
		using cursor_cref = clang::cursor const&;

		namespace analyze_impl
		{
			template <class U>
			bool analyze(cursor_cref c, group_cref g, vim_ref vim)
			{
				auto it = g.find(U::group());
				if (it == end(g)) return false;
				if (!U::applicable(c)) return false;

				log << "\t\t<" << U::group() << ">\n";
				auto& kind = it->second;
				for (auto& region: U::apply(c))
					vim.highlight(kind, region);
				return true;
			}

			template <class U, class T, class ...TS>
			bool analyze(cursor_cref c, group_cref g, vim_ref vim)
			{
				return (analyze<U       >(c, g, vim) ||
						analyze<T, TS...>(c, g, vim));
			}

			template <class T, class ...TS>
			void analyze(cursor_cref c, group_cref g, vim_ref vim, std::tuple<T, TS...>)
			{
				if (analyze<T, TS...>(c, g, vim)) return;

				auto it = g.find(c.kind);
				if (it == end(g)) return;

				auto& kind = it->second;
				vim.highlight(kind, { c.head.y, c.head.x, c.tail.x-c.head.x });
			}
		}

		void analyze(cursor_cref c, group_cref g, vim_ref vim)
		{
			analyze_impl::analyze(c, g, vim, all_analyzer_rules{});
		}
	}

	void analyzer::parse(source_cref src, group_cref group, vim_ref vim)
	{
		tu.parse(src);

		// analyze errors
		auto diags = tu.diagnostics();
		log << "analyzer::parse():\n";
		for (auto& diag: diags)
			try {
				log << "\t[error]\n"
					<< "\t\t" << diag.text() << '\n';

				auto loc = diag.location();
				if (!loc.is_from_main()) continue;

				auto pos =  loc.position();
				vim.highlight(group.at(":error"), { pos.y, pos.x, 1 });
				log << "\t\t" << pos.y << ", " << pos.x << '\n';

				auto ranges = diag.ranges();
				for (auto& range: ranges) {
					auto head = range.head();
					if (!head.is_from_main()) continue;

					auto head_pos =       head  .position();
					auto tail_pos = range.tail().position();

					vim.highlight(group.at(":error_range"), { head_pos.y, head_pos.x, 1 });
					log << "\t\t" << head_pos.y << ", " << head_pos.x
						<< " -> " << tail_pos.y << ", " << tail_pos.x
						<< "\n";
				}
			}
			catch (std::out_of_range) {}

		// semantic highlighting
		tu.cursor().each_child([&](cursor_cref c) {
			if (!c.is_from_main) return false;

			log << "\t[" << c.kind << "]\n"
				<< "\t\t\"" << c.name << "\"\n"
				<< "\t\t" << c.head.y << ", " << c.head.x
				<< " -> " << c.tail.y << ", " << c.tail.x
				<< " @ " << c.pos.y << ", " << c.pos.x << '\n';
			log << "\t\t{" << c.file() << "}\n";

			auto ref = c.reference();
			log << "\t\t::[" << ref.kind << "]\n"
				<< "\t\t\t\"" << ref.name << "\"\n"
				<< "\t\t\t{" << ref.file() << "}\n";

			analyze(c, group, vim);

			return true;
		});
	}
}

