#pragma once
#include "c.hh"
#include "internal.hh"
#include "unsaved_file.hh"
#include "index.hh"
#include "cursor.hh"

namespace clang
{
	struct translation_unit : public internal::guard<c::translation_unit::type>
	{
		using self_type = translation_unit;
		using super_type = internal::guard<c::translation_unit::type>;
		using source_type = unsaved_file::source_type;
		using index_type = clang::index;

		translation_unit(const index_type& index, const source_type& src="")
		{
			clang::unsaved_file f(src);
			const char* argv[] = { "-std=gnu++11" };
			set(c::translation_unit::create_from_source_file(
					index, f.name(), 1, argv, 1, f));
		}

		~translation_unit() override { if (owned) c::translation_unit::dispose(get()); }

		void parse(const source_type& src)
		{
			clang::unsaved_file f(src);
			auto opt = c::options::default_reparse(get());
			c::translation_unit::reparse(get(), 1, f, opt);
		}

		cursor cursor() { return c::translation_unit::get_cursor(get()); }
	};
};

