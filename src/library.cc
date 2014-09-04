#include "library.hh"
#include "worker.hh"
#include <utility>

namespace vimlight
{
	namespace library
	{
		static lua::library lib;
		static bool inited = false;

		int open(lua::state& s)
		{
			lib["init"] = [](lua::state& s) {
				if (!inited) {
					worker::filename_type hlgroup;
					s.get(hlgroup, 1);
					worker::start(hlgroup);
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
				for (decltype(cmds.size()) i=0; i<cmds.size(); i++) {
					s.push(cmds[i]);
					s.field(-2, i+1);
				}
				return 1;
			};

			lib["name"] = [](lua::state& s) {
				if (!inited) s.error("not initialized");

				worker::filename_type f;
				s.get(f, 1);
				worker::name(std::move(f));
				return 0;
			};

			s.push(lib);
			return 1;
		}
	}
}

