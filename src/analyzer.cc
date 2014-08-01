#include "clang/all.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

string read_all(istream& ist)
{
	stringstream ss;
	ss << ist.rdbuf();
	return ss.str();
}

using hlgroup_type = std::unordered_map<std::string, std::string>;
static hlgroup_type hlgroup {
	{ "TypeRef", "Type" },
	{ "NamespaceRef", "Namespace" },
	{ "TemplateRef", "Template" },
};

using pos_type = struct clang::location::position;
using group_type = std::string;
void highlight(pos_type head, pos_type tail, const group_type& group)
{
	cout
		<< head.y << ',' << head.x << ','
		<< tail.y << ',' << tail.x << ','
		<< '\'' << group << '\''
		<< endl;
}

int main(int argc, char* argv[])
{
	string src;
	if (argc == 1)
		src = read_all(cin);
	else if (argc == 2) {
		ifstream f{argv[1]};
		src = read_all(f);
	}
	else {
		struct bad_arguments {};
		throw bad_arguments{};
	}

	clang::index index;
	clang::translation_unit tu(index);
	tu.parse(src);
	auto cu = tu.cursor();

	cu.each_child([](const clang::cursor& cursor) {
		auto range = cursor.range();
		auto head = range.head();
		if (!head.is_from_main())
			return false;

		auto head_pos =       head  .position();
		auto tail_pos = range.tail().position();

		auto kind = cursor.kind();
		auto name = kind.name();

#ifdef DEBUG
		cout << "\e[1;34m" << name << "    \t\e[0;34m" << cursor.name() << "\e[0m" << endl;
#endif

		try {
			highlight(head_pos, tail_pos, hlgroup.at(name));
			return true;
		}
		catch (std::out_of_range) {}

		return true;
	});
}

