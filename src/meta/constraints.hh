#pragma once

namespace meta
{
	inline namespace constraints
	{
		struct only_movable
		{
			only_movable() = default;
			only_movable(only_movable const&) = delete;
			only_movable(only_movable     &&) = default;
			only_movable & operator = (only_movable const&) = delete;
			only_movable & operator = (only_movable     &&) = default;
		};

		struct non_transferable
		{
			non_transferable() = default;
			non_transferable(non_transferable const&) = delete;
			non_transferable(non_transferable     &&) = delete;
			non_transferable & operator = (non_transferable const&) = delete;
			non_transferable & operator = (non_transferable     &&) = delete;
		};
	}
}

