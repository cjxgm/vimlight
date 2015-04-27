#pragma once
#include <fstream>

namespace vimlight
{
	namespace no_log
	{
		struct log {};
		template <class T>
		log& operator<<(log& l, T&&) { return l; }
	}

	namespace file_log
	{
		struct log
		{
			std::ofstream o{"/tmp/vimlight.log"};
		};
		template <class T>
		log& operator<<(log& l, T&& what)
		{
			l.o << what;
			return l;
		}
	}

#ifdef RELEASE
	namespace log_system = no_log;
#else
	namespace log_system = file_log;
#endif

	extern log_system::log log;
}

