#pragma once
#include "../log.hh"
#include "c.hh"
#include "resource.hh"
#include "unsaved_file.hh"
#include "index.hh"
#include "cursor.hh"
#include "diagnostic.hh"
#include "option_parser.hh"
#include <vector>
#include <utility>

namespace clang
{
	struct translation_unit : public resource::unique<c::translation_unit::type>
	{
		using self_type = translation_unit;
		using super_type = unique;
		using source_type = unsaved_file::source_type;
		using filename_type = unsaved_file::name_type;
		using option_type = filename_type;
		using index_type = clang::index;
		using diagnostics_type = std::vector<clang::diagnostic>;

		translation_unit(index_type& index)
			: super_type(c::translation_unit::dispose), index(index)
		{
			// use c++14 as default
			setup("source.cc", "-std=gnu++14 -Wall -Wextra");
		}

		void setup(filename_type const& f, option_type const& o)
		{
			clang::unsaved_file uf(f, "");
			option_parser op{o};

			set(c::translation_unit::parse(
					index, f.c_str(), op.argv(), op.argc(), uf, 1,
					c::translation_unit::flag::none));

			file = f;
		}

		void parse(source_type const& src)
		{
			clang::unsaved_file uf(file, src);
			auto opt = c::options::default_reparse(get());
			c::translation_unit::reparse(get(), 1, uf, opt);

			visualize();
		}

		cursor cursor() const { return c::translation_unit::get_cursor(get()); }

		diagnostics_type diagnostics() const
		{
			diagnostics_type diags;
			auto size = c::diagnostic::get_count(get());
			diags.reserve(size);
			for (decltype(size) i=0; i<size; i++)
				diags.emplace_back(c::diagnostic::get(get(), i));
			return std::move(diags);
		}

		void visualize()
#ifdef LOG
		;
#else
		{}
#endif

	private:
		index_type& index;
		filename_type file;
	};
}

