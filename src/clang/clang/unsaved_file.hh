#pragma once
#include "c.hh"
#include <string>

namespace clang
{
	struct unsaved_file
	{
		using self_type = unsaved_file;
		using value_type = c::unsaved_file::type;
		using source_type = std::string;
		using name_type = const char*;

		unsaved_file(const source_type& src) : src(src) { update(); }

		unsaved_file(const self_type& other) : src(other.src) { update(); }
		self_type& operator=(const self_type& other)
		{
			src = other.src;
			update();
			return *this;
		}

		const value_type* get() const { return &value; }
		      value_type* get()       { return &value; }
		operator value_type*() { return get(); }

		name_type name() const { return "source.cc"; }

	private:
		source_type src;
		value_type value;

		void update()
		{
			value.Filename = name();
			value.Contents = src.c_str();
			value.Length   = src.size();
		}
	};
};

