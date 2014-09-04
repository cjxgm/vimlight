#pragma once
// analyzer analyzes source code and produce a highlight list.
#include "clang/all.hh"
#include "highlight/all.hh"

namespace vimlight
{
	struct analyzer
	{
		using source_type = clang::translation_unit::source_type;
		using filename_type = clang::translation_unit::filename_type;
		using group_type = highlight::group;
		using list_type = highlight::list;

		analyzer() : index(), tu(index) {}

		list_type parse(const source_type& src, const group_type& group);
		void name(const filename_type& file) { tu.name(file); }

	private:
		clang::index index;
		clang::translation_unit tu;
	};
}

