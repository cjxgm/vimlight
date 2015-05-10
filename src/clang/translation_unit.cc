#ifndef RELEASE

#include "translation_unit.hh"
#include "../log.hh"
#include <fstream>
using vimlight::log;

namespace
{
	void replace(std::string& s, std::string const& from, std::string const& to)
	{
		using size_type = std::string::size_type;
		for (size_type pos = 0;
				(pos = s.find(from, pos)) != s.npos;
				pos += to.size())
			s.replace(pos, from.size(), to);
	}

	void visualize(std::ofstream & dot, clang::cursor const& c)
	{
		if (!c.is_from_main) return;

		auto ref = c.reference();
		auto id = c.identifier();
		auto oc = c.first_child();
		auto name = c.name;
		auto ref_name = ref.name;

		bool highlight = (c.kind == "call-expr") &&
			oc && oc.get().reference().kind != "invalid-file" &&
			ref.kind != "invalid-file" &&
			(name.size() < 8 ? true : name.substr(0, 8) != "operator");

		replace(name, "\\", "\\\\");
		replace(name, "\"", "\\\"");
		replace(ref_name, "\"", "\\\"");
		dot << "\n\t\"" << id << "\" [label=\""
			<< "[" << c.kind << "] \\\"" << name << "\\\"\\n"
			<< c.head.y << ", " << c.head.x << " -> "
			<< c.tail.y << ", " << c.tail.x << " @ "
			<< c.pos.y << ", " << c.pos.x << "\\n"
			<< "[" << ref.kind << "] \\\"" << ref_name << "\\\"" << "\""
			<< (highlight ? " color=red fontcolor=red" : "") << "]\n";

		c.each_child([&dot, &id](clang::cursor const& c) {
			if (!c.is_from_main) return false;
			dot << "\t\"" << id << "\" -> \"" << c.identifier() << "\"\n";
			return false;
		});

		c.each_child([&dot](clang::cursor const& c) {
			visualize(dot, c);
			return false;
		});
	}
}


void clang::translation_unit::visualize()
{
	std::ofstream dot{"/tmp/vimlight.dot"};
	if (!dot) return;
	dot << "digraph {\n"
		<< "\tnode [fontname=\"monospace\"]\n"
		<< "\tnode [shape=box]\n"
		<< "\tnode [width=0]\n";
	::visualize(dot, cursor());
	dot << "}\n\n";
}

#endif

