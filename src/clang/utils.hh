#pragma once
#include <string>

namespace clang
{
	inline namespace utils
	{
		std::string uncamel(std::string const& x, std::string const& delimiter="-");
	}
}

