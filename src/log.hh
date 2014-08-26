#pragma once
#include <fstream>

namespace vimlight
{
	namespace no_log
	{
		struct log {};
		namespace helper
		{
			template <class T>
			log& operator<<(log& l, T&&) { return l; }
		};
	};

	namespace file_log
	{
		struct log
		{
			std::ofstream o{"/tmp/vimlight.log"};
		};
		namespace helper
		{
			template <class T>
			log& operator<<(log& l, T&& what)
			{
				l.o << what;
				return l;
			}
		};
	};

#ifndef LOG_SYSTEM
#define LOG_SYSTEM no_log
#endif

	namespace log_system = LOG_SYSTEM;
	extern log_system::log log;
};

using namespace vimlight::log_system::helper;

