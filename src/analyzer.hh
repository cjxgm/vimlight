#pragma once
#include "clang/all.hh"
#include "highlight/all.hh"

namespace vimlight
{
	struct analyzer
	{
		using source_type = clang::translation_unit::source_type;
		using group_type = highlight::group;
		using list_type = highlight::list;

		analyzer() : index(), tu(index) {}

		list_type parse(const source_type& src, const group_type& group);

	private:
		clang::index index;
		clang::translation_unit tu;
	};
};

