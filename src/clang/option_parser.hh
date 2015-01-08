#pragma once
#include <string>
#include <vector>

namespace clang
{
	// parse option strings into argc/argv style arguments
	// "hello world a=b" => {"hello", "world", "a=b", nullptr}
	// "hello\\ world\ a=b" => {"hello\", "world a=b", nullptr}
	// "hello world a=b\" => {"hello", "world", "a=b\", nullptr}
	// "hello world a=b\\" => {"hello", "world", "a=b\", nullptr}
	// "hello world a=b\ " => {"hello", "world", "a=b ", nullptr}
	// "hello world a=b " => {"hello", "world", "a=b", nullptr}
	struct option_parser
	{
		using option_type = std::string;
		using option_list = std::vector<option_type>;
		using raw_option_type = char const*;
		using raw_option_list = std::vector<raw_option_type>;

		option_parser(option_type const& o)
		{
			tokenize(o);
			populate_raw_options();
		}

		auto argv() const { return raw_opts.data(); }
		auto argc() const { return     opts.size(); }

	private:
		option_list opts;
		raw_option_list raw_opts;
		// raw_opts is argv-like, with one more element of nullptr

		void tokenize(option_type const& o);
		void populate_raw_options();
	};
}

