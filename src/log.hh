#pragma once
#include <fstream>

namespace vimlight
{
	using log_file = std::ofstream;
	using std::endl;
	extern log_file log;
};

