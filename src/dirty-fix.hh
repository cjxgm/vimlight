// dirty fixes
// they are only the temporary solutions to the problem.
// the proper solution still need to be found.
#pragma once
#include <unistd.h>
#include "meta/constraints.hh"

namespace dirty_fix
{
	// fix GitHub Issue #22: The terminal is messed up in the latest Arch Linux
	//
	// it seems that redirect stderr to /dev/null solves the problem,
	// so is closing stderr.
	//
	// thus, this fixer will close stderr in its object scope.
	// that is, close stderr on construction, and restore stderr on destruction.
	struct clang_fixer : meta::non_transferable
	{
		clang_fixer()
		{
			old_stderr = dup(STDERR_FILENO);
			close(STDERR_FILENO);
		}

		~clang_fixer()
		{
			dup2(old_stderr, STDERR_FILENO);
			close(old_stderr);
		}

	private:
		int old_stderr;
	};
}

