#include "library.hh"
#include "worker.hh"
#include <utility>

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
				else s.error("multiple initialization");
				return 0;
			};

			lib["request"] = [](lua::state& s) {
				if (!inited) s.error("not initialized");
				worker::source_type src;
				s.get(src, 1);
				worker::request(std::move(src));
				return 0;
			};

			lib["get"] = [](lua::state& s) {
				if (!inited) s.error("not initialized");
				if (!worker::done()) return 0;
				auto cmds = worker::get();
				s.table(cmds.size(), 0);
				for (int i=0; i<cmds.size(); i++) {
					s.push(cmds[i]);
					s.field(-2, i+1);
				}
				return 1;
			};

			s.push(lib);
			return 1;
		}
	};
};

