#pragma once
#include "type.hh"

namespace vimlight
{
	namespace highlight
	{
		struct record
		{
			using coord_type = unsigned;
			using  name_type = vim_name_type;
			coord_type y1;
			coord_type x1;
			coord_type y2;
			coord_type x2;
			 name_type name;
		};

		inline namespace record_helper
		{
			bool operator==(const record& lhs, const record& rhs)
			{
				return
					(lhs.y1 == rhs.y1) &&
					(lhs.x1 == rhs.x1) &&
					(lhs.y2 == rhs.y2) &&
					(lhs.x2 == rhs.x2) &&
					(lhs.name == rhs.name);
			}

			bool operator!=(const record& lhs, const record& rhs)
			{
				return
					(lhs.y1 != rhs.y1) ||
					(lhs.x1 != rhs.x1) ||
					(lhs.y2 != rhs.y2) ||
					(lhs.x2 != rhs.x2) ||
					(lhs.name != rhs.name);
			}
		};
	};
};

