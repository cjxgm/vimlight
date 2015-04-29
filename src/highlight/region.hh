#pragma once
#include "type.hh"
#include <initializer_list>

namespace vimlight
{
	namespace highlight
	{
		struct region
		{
			coord_type y;
			coord_type x;
			coord_type w;
		};

		using regions = std::initializer_list<region>;
	}
}

