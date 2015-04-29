#include <utility>
#include <string>
#include <algorithm>

namespace vimlight
{
	namespace analyzer_utils
	{
		inline bool is_identifier(char c)
		{
			return (('a' <= c && c <= 'z') ||
					('A' <= c && c <= 'Z') ||
					('0' <= c && c <= '9') ||
					c == '_' || c == '$');
		}

		inline int identifier_length(std::string const& x)
		{
			if (x.empty()) return 0;
			auto first = begin(x);
			if (*first == '~') ++first;
			auto it = std::find_if_not(first, end(x), is_identifier);
			return it - begin(x);
		}

		inline bool is_operator(std::string const& x)
		{
			constexpr auto& op = "operator";
			constexpr auto op_size = sizeof(op)-1;	// -1 for trailing '\0'
			if (x.size() <= op_size) return false;
			if (x.substr(0, op_size) != op) return false;
			return !is_identifier(x[op_size]);
		}
	}
}

