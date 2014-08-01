#include "vim.hh"
#include "lua/all.hh"
#include "highlight/all.hh"
#include <iostream>
using std::cout;
using std::endl;

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

	hl::list list;
	list.push_back({1, 2, 3, 4, "hello"});
	list.push_back({3, 7, 3, 9, "world"});
	delta.update(std::move(list), vim);
	cout << "-----------------------------" << endl;

	list.push_back({1, 2, 3, 4, "hello"});
	list.push_back({3, 7, 3, 9, "world"});
	delta.update(std::move(list), vim);
	cout << "-----------------------------" << endl;

	list.push_back({1, 2, 3, 4, "hello"});
	delta.update(std::move(list), vim);
	cout << "-----------------------------" << endl;

	list.push_back({1, 2, 3, 4, "hello"});
	list.push_back({3, 7, 3, 9, "world"});
	delta.update(std::move(list), vim);
	cout << "-----------------------------" << endl;

	list.push_back({3, 7, 3, 9, "world"});
	delta.update(std::move(list), vim);
	cout << "-----------------------------" << endl;

	list.push_back({1, 2, 3, 4, "hello"});
	list.push_back({3, 7, 3, 9, "world"});
	delta.update(std::move(list), vim);
	cout << "-----------------------------" << endl;

	hl::group group("hlgroup");
	cout << group.at("hello") << endl;
	cout << group.at("world") << endl;
	cout << group.at("yes") << endl;
}

