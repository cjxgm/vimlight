#include "library.hh"
#include "worker.hh"

static lua::library lib;
static bool inited = false;

namespace vimlight
{
	namespace library
	{
		int open(lua::state& s)
		{
			lib["init"] = [](lua::state& s) {
				if (!inited) {
					worker::command_func_type cmd;
					s.get(cmd, 1);
					worker::run(cmd);
					inited = true;
				}
				return 0;
			};

			lib["request"] = [](lua::state& s) {
				if (!inited) s.error("not initialized.");
				worker::source_type src;
				s.get(src, 1);
				worker::request(src);
				return 0;
			};

			s.push(lib);
			return 1;
		}
	};
};

