#pragma once
// analyzer analyzes source code and produce a highlight list.
#include "../clang/translation_unit.hh"
#include "../highlight/group.hh"
#include "../vim.hh"

namespace vimlight
{
	struct analyzer
	{
		using   source_type =     clang::translation_unit::  source_type;
		using filename_type =     clang::translation_unit::filename_type;
		using   option_type =     clang::translation_unit::  option_type;
		using    group_type = highlight::group;
		using      vim_type =  vimlight::vim;

		using   source_cref =   source_type const&;
		using filename_cref = filename_type const&;
		using   option_cref =   option_type const&;
		using    group_cref =    group_type const&;
		using       vim_ref =           vim      &;

		analyzer() : index(), tu(index) {}

		void parse(source_cref src, group_cref group, vim_ref vim);
		void setup(filename_cref file, option_cref opt) { tu.setup(file, opt); }

	private:
		clang::index index;
		clang::translation_unit tu;
	};
}

