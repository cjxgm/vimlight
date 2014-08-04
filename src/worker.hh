#pragma once
#include "analyzer.hh"
#include "vim.hh"
#include "highlight/type.hh"

namespace vimlight
{
	namespace worker
	{
		using   source_type =  analyzer::  source_type;
		using commands_type =       vim::commands_type;
		using filename_type = highlight::filename_type;

		// should be called once and only once before calling any
		// other functions in worker namespace.
		//
		// this function will start a worker thread in the background.
		void start(const filename_type& hlgroup);

		// request the worker to highlight the source.
		void request(source_type src);

		// check if the last request is done
		bool done();

		// get the highlight result
		commands_type get();
	};
};

