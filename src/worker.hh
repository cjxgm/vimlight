#pragma once
#include "vim.hh"
#include "analyzer.hh"

namespace vimlight
{
	namespace worker
	{
		using command_func_type = vim::command_func_type;
		using source_type = analyzer::source_type;

		void run(command_func_type cmd);
		void request(const source_type& src);
	};
};

