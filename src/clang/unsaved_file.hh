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

		unsaved_file(const name_type& file, const source_type& src)
			: file(file), src(src)
		{
			update();
		}

		unsaved_file(const self_type& other) = delete;
		self_type& operator=(const self_type& other) = delete;

		const value_type* get() const { return &value; }
		      value_type* get()       { return &value; }
		operator value_type*() { return get(); }

		const name_type& name() const { return file; }

	private:
		const name_type& file;
		const source_type& src;
		value_type value;

		void update()
		{
			value.Filename = name().c_str();
			value.Contents = src.c_str();
			value.Length   = src.size();
		}
	};
}

