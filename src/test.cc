#include "vim.hh"
#include "lua/all.hh"
#include "highlight/all.hh"
#include "analyzer.hh"
#include <iostream>
#include <string>
#include <sstream>
using std::cout;
using std::endl;

std::string read_all(std::istream& ist)
{
	std::stringstream ss;
	ss << ist.rdbuf();
	return ss.str();
}

int main()
{
	vimlight::vim vim([](lua::raw_state* rs) {
		lua::state s(rs);
		lua::string cmd;
		s.get(cmd, 1);
		cout << cmd << endl;
		return 0;
	});

	namespace hl = vimlight::highlight;

	hl::delta delta;
	hl::group group("hlgroup");
	vimlight::analyzer analyzer;

	auto src = read_all(std::cin);
	auto result = analyzer.parse(src, group);
	delta.update(result, vim);
}

