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

		translation_unit(const index_type& index, const source_type& src)
		{
			clang::unsaved_file f(src);
			set(c::translation_unit::create_from_source_file(
					index, f.name(), 0, nullptr, 1, f));
		}

		~translation_unit() override { if (owned) c::translation_unit::dispose(get()); }

		cursor cursor() { return clang::cursor(c::translation_unit::get_cursor(get())); }
	};
};

