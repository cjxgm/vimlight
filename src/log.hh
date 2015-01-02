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

#define CAT2(X, Y)		X ## Y
#define CAT(X, Y)		CAT2(X, Y)

#ifdef LOG
#	define LOG_SYSTEM	CAT(LOG, _log)
#else
#	define LOG_SYSTEM	no_log
#endif

	namespace log_system = LOG_SYSTEM;

#undef LOG_SYSTEM
#undef CAT
#undef CAT2

	extern log_system::log log;
}

