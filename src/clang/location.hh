#pragma once
#include "c.hh"
#include "resource.hh"
#include "string.hh"
#include <string>

namespace clang
{
	struct location : resource::bin<c::location::type>
	{
		using self_type = location;
		using super_type = bin;
		using name_type = std::string;

		struct position
		{
			using coord_type = int;
			coord_type y;
			coord_type x;
		};

		location(value_type value) : super_type(value) {}

		bool is_from_main() const
		{
			return c::location::is_from_main_file(get());
		}

		position position() const
		{
			struct position pos;
			c::location::get_spelling(get(), nullptr,
					reinterpret_cast<unsigned*>(&pos.y),
					reinterpret_cast<unsigned*>(&pos.x),
					nullptr);
			return pos;
		}

		name_type file() const
		{
			c::file::type f;
			c::location::get_file(get(), &f, nullptr, nullptr, nullptr);
			if (f) return clang::string{c::file::get_name(f)};
			return "";
		}
	};
}

