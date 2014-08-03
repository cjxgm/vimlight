#include "library.hh"

extern "C"
{
	int luaopen_vimlight(lua::raw_state* rs)
	{
		lua::state s(rs);
		return vimlight::library::open(s);
	}
};

