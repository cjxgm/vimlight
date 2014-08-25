#pragma once
#include "c.hh"
#include "internal.hh"
#include "unsaved_file.hh"
#include "index.hh"
#include "cursor.hh"
#include "diagnostic.hh"
#include "../log.hh"
#include <vector>
#include <utility>

namespace clang
{
	struct translation_unit : public internal::guard<c::translation_unit::type>
	{
		using self_type = translation_unit;
		using super_type = internal::guard<c::translation_unit::type>;
		using source_type = unsaved_file::source_type;
		using filename_type = unsaved_file::name_type;
		using index_type = clang::index;
		using diagnostics_type = std::vector<clang::diagnostic>;

		translation_unit(index_type& index) : index(index)
		{
			name("source.cc");
		}

		~translation_unit() override { if (owned) c::translation_unit::dispose(get()); }

		void name(const filename_type& f)
		{
			clang::unsaved_file uf(f, "");
			const char* argv[] = { "-std=gnu++1y" };
			set(c::translation_unit::parse(
					index, f.c_str(), argv, 1, uf, 1,
					c::translation_unit::flag::none));
			file = f;
		}

		void parse(const source_type& src)
		{
			clang::unsaved_file uf(file, src);
			auto opt = c::options::default_reparse(get());
			c::translation_unit::reparse(get(), 1, uf, opt);
		}

		cursor cursor() const { return c::translation_unit::get_cursor(get()); }

		diagnostics_type diagnostics() const
		{
			diagnostics_type diags;
			auto size = c::diagnostic::get_count(get());
			using vimlight::log;
			using vimlight::endl;
			log << "clang::tu::diagnostics(): size=" << size << endl;
			diags.reserve(size);
			for (int i=0; i<size; i++)
				diags.emplace_back(c::diagnostic::get(get(), i));
			return std::move(diags);
		}
	private:
		index_type& index;
		filename_type file;
	};
};

