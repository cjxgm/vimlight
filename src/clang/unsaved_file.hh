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
		using name_type = std::string;

		unsaved_file(name_type const& file, source_type const& src)
			: file(file), src(src)
		{
			update();
		}

		unsaved_file(self_type const& other) = delete;
		self_type& operator=(self_type const& other) = delete;

		value_type const* get() const { return &value; }
		      value_type* get()       { return &value; }
		operator value_type*() { return get(); }

		name_type const& name() const { return file; }

	private:
		name_type const& file;
		source_type const& src;
		value_type value;

		void update()
		{
			value.Filename = name().c_str();
			value.Contents = src.c_str();
			value.Length   = src.size();
		}
	};
}

