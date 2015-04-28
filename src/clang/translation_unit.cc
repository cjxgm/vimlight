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

	void visualize(std::ofstream & dot, clang::cursor c)
	{
		if (!c.range().head().is_from_main()) return;

		auto ref = c.reference();
		auto ref_kind = ref.kind().name();
		auto ref_name = ref.name();

		auto range = c.range();
		auto head = range.head().position();
		auto tail = range.tail().position();
		auto loc  = c.location().position();
		auto kind = c.kind().name();
		auto name = c.name();
		auto id = c.identifier();
		replace(name, "\\", "\\\\");
		replace(name, "\"", "\\\"");
		replace(ref_name, "\"", "\\\"");
		dot << "\n\t\"" << id << "\" [label=\""
			<< "[" << kind << "] \\\"" << name << "\\\"\\n"
			<< head.y << ", " << head.x << " -> "
			<< tail.y << ", " << tail.x << " @ "
			<< loc.y << ", " << loc.x << "\\n"
			<< "[" << ref_kind << "] \\\"" << ref_name << "\\\""
			<< "\"]\n";

		c.each_child([&dot, &id](clang::cursor c) {
			if (!c.range().head().is_from_main()) return false;
			dot << "\t\"" << id << "\" -> \"" << c.identifier() << "\"\n";
			return false;
		});

		c.each_child([&dot](clang::cursor c) {
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
		<< "\tnode [fontname=\"Monospace\"]\n"
		<< "\tnode [shape=box]\n"
		<< "\tnode [width=0]\n";
	::visualize(dot, cursor());
	dot << "}\n\n";
}

#endif

