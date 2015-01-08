#include <sstream>
#include "option_parser.hh"

#include "../log.hh"
using vimlight::log;



namespace clang
{
	void option_parser::tokenize(option_type const& o)
	{
		log << "option_parser: " << o << "\n";

		std::stringstream ss{o};
		std::string token;

		// remove leading spaces
		while (ss.peek() == ' ') ss.get();

		while (true) {
			char c;
			if (!ss.get(c)) break;

			switch (c) {
				case ' ':
					// skip rest spaces
					while (ss.peek() == ' ') ss.get();
					opts.push_back(std::move(token));
					break;
				case '\\':
					if (ss.get(c)) token += c;
					else token += '\\';
					break;
				default:
					token += c;
					break;
			}
		}
		if (token.size()) opts.push_back(std::move(token));
	}

	void option_parser::populate_raw_options()
	{
		for (auto& opt: opts)
			raw_opts.push_back(opt.data());
		raw_opts.push_back({});

		for (auto& ropt: raw_opts)
			log << "\t" << (ropt ? ropt : "{}") << "\n";
	}
}

