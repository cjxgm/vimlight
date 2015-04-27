#pragma once
// analyzer analyzes source code and produce a highlight list.
#include "clang/translation_unit.hh"
#include "highlight/group.hh"
#include "highlight/list.hh"

namespace vimlight
{
	struct analyzer
	{
		using source_type = clang::translation_unit::source_type;
		using filename_type = clang::translation_unit::filename_type;
		using option_type = clang::translation_unit::option_type;
		using group_type = highlight::group;
		using list_type = highlight::list;

		analyzer() : index(), tu(index) {}

		list_type parse(source_type const& src, group_type const& group);
		void setup(filename_type const& file, option_type opt) { tu.setup(file, opt); }

	private:
		clang::index index;
		clang::translation_unit tu;
	};
}

