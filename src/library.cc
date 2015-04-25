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
				s.push(cmds);
				return 1;
			};

			lib["setup"] = [](lua::state& s) {
				if (!inited) s.error("not initialized");

				worker::filename_type f;
				worker::option_type o;
				s.get(f, 1);
				s.get(o, 2);
				worker::setup(std::move(f), std::move(o));
				return 0;
			};

			// !!! this can only be your last call into the library !!!
			// this is only for proper shutdown
			lib["exit"] = [](lua::state& s) {
				if (!inited) s.error("not initialized");
				worker::stop();
				// inited will remain true
				// you should never ever call anything afterwards
				return 0;
			};

			s.push(lib);
			return 1;
		}
	}
}

