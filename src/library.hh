// vimlight_engine is a lua library.
// this module implements all the functions exposed to lua.
#pragma once
#include "lua/lua.hh"

namespace vimlight
{
	namespace library
	{
		int open(lua::state& s);
	}
}

