#pragma once
// management and communication with/to/from the worker thread
// the worker thread parses the requested source code and
// produces vim highlighting commands.
#include <string>
#include "analyzer/analyzer.hh"
#include "vim.hh"
#include "highlight/type.hh"

namespace vimlight
{
	namespace worker
	{
		using   source_type =  analyzer::source_type;
		using      commands =       vim::commands;
		using filename_type = highlight::filename_type;
		using   option_type =  analyzer::option_type;

		// should be called once and only once before calling any
		// other functions in worker namespace.
		//
		// this function will start a worker thread in the background.
		void start(filename_type const& hlgroup);

		// request the worker to highlight the source.
		// you can call request even if !done()
		void request(source_type src);

		// check if the last request is done
		// when it's done, you can call get() to get the result.
		bool done();

		// get the highlight result if done()
		// returns vim commands
		commands get();

		// rename the current editing source file, and/or
		// change the current command line options
		void setup(filename_type f, option_type o);

		// signal and wait the worker thread to exit
		void stop();
	}
}

