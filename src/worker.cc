#include "worker.hh"
#include "channel.hh"
#include "highlight/all.hh"
#include "log.hh"
#include <thread>
#include <utility>

namespace vimlight
{
	namespace worker
	{
		static channel chn_worker;
		static channel chn_main;

		struct event_done : public channel::event {};
		struct event_request : public channel::event
		{
			source_type src;
			event_request(source_type src) : src{std::move(src)} {}
		};
		struct event_result : public channel::event
		{
			commands_type cmds;
			event_result(commands_type cmds) : cmds{std::move(cmds)} {}
		};
		struct event_name : public channel::event
		{
			filename_type name;
			event_name(filename_type name) : name{std::move(name)} {}
		};


		void start(const filename_type& hlgroup)
		{
			auto rm_done = chn_main.listen<event_done>([] {});

			std::thread th([&hlgroup] {
				vimlight::vim vim;
				vimlight::analyzer analyzer;
				highlight::group group(hlgroup);
				highlight::delta delta;
				chn_main.post(event_done{});

				chn_worker.listen<event_request>([&](event_request ev) {
					log << "(worker) parse request" << endl;
					auto result = analyzer.parse(ev.src, group);
					delta.update(result, vim);
					chn_main.post(event_done{});
					chn_main.post(event_result{std::move(vim.get())});
				});

				chn_worker.listen<event_name>([&](event_name ev) {
					log << "(worker) name: " << ev.name << endl;
				});

				while (true) chn_worker.wait();
			});
			th.detach();

			chn_main.wait();
			rm_done();
		}

		void request(source_type src)
		{
			chn_worker.post(event_request{std::move(src)});
		}

		bool done()
		{
			bool is_done = false;
			auto rm_done = chn_main.listen<event_done>([&] {
				is_done = true;
			});
			chn_main.poll();
			rm_done();
			return is_done;
		}

		commands_type get()
		{
			commands_type cmds;
			auto rm_result = chn_main.listen<event_result>(
					[&](event_result ev) {
						cmds = std::move(ev.cmds);
					});
			chn_main.wait();
			rm_result();
			return std::move(cmds);
		}

		void name(filename_type f)
		{
			chn_worker.post(event_name{std::move(f)});
		}
	};
};

