#include "utils.hh"
#include <algorithm>

namespace clang
{
	inline namespace utils
	{
		namespace
		{
			bool is_upper(char x)
			{
				return ('A' <= x && x <= 'Z');
			}

			char to_lower(char x)	// assert(is_upper(x))
			{
				return x - 'A' + 'a';
			}
		}

		// assume x only consists alphabets.
		std::string uncamel(std::string const& x, std::string const& delimiter)
		{
			std::string r;
			r.reserve(x.size());

			bool need_delimiter = true;
			for (auto it=begin(x); it!=end(x); ++it) {
				auto c = *it;
				auto nit = it+1;
				if (is_upper(c)) {
					if ((nit!=end(x) && !is_upper(*nit)) || need_delimiter)
						if (!r.empty()) r += delimiter;
					r += to_lower(c);
					need_delimiter = false;
				}
				else {
					r += c;
					need_delimiter = true;
				}
			}

			return r;
		}
	}
}

