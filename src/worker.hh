#pragma once
#include "vim.hh"
#include "analyzer.hh"

namespace vimlight
{
	namespace worker
	{
		using command_func_type = vim::command_func_type;
		using source_type = analyzer::source_type;

		// should be called once and only once before calling any
		// other functions in worker namespace.
		//
		// this function will run a worker thread in the background.
		void run(command_func_type cmd);

		// request the worker to highlight the source.
		void request(const source_type& src);
	};
};

