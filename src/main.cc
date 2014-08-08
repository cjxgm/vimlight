// this is a lua library.
// this module is a bridge to the c++ implementation of the library.
#include "library.hh"

extern "C"
{
	int luaopen_vimlight(lua::raw_state* rs)
	{
		lua::state s(rs);
		return vimlight::library::open(s);
	}
};

