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
		static std::thread th;

		struct event_done : public channel::event { event_done() : event{3} {} };
		struct event_request : public channel::event
		{
			source_type src;
			event_request(source_type src) : event{2}, src{std::move(src)} {}
		};
		struct event_result : public channel::event
		{
			commands_type cmds;
			event_result(commands_type cmds) : event{1}, cmds{std::move(cmds)} {}
		};
		struct event_setup : public channel::event
		{
			filename_type name;
			option_type option;
			event_setup(filename_type name, option_type option)
				: event{3}, name{std::move(name)}, option{std::move(option)} {}
		};
		struct event_stop : public channel::event { event_stop() : event{4} {} };


		void start(filename_type const& hlgroup)
		{
			auto rm_done = chn_main.listen<event_done>([] {});

			th = std::thread{[&hlgroup] {
				vimlight::vim vim;
				vimlight::analyzer analyzer;
				highlight::group group(hlgroup);
				highlight::collector collector;
				chn_main.post(event_done{});

				chn_worker.listen<event_request>([&](auto ev) {
					log << "(worker) parse request\n";
					auto result = analyzer.parse(ev.src, group);
					collector.update(result, vim);
					chn_main.post(event_done{});
					chn_main.post(event_result{std::move(vim.get())});
				});

				chn_worker.listen<event_setup>([&](auto ev) {
					log << "(worker) setup request\n";
					log << "\t" << ev.name << '\n';
					log << "\t" << ev.option << '\n';
					analyzer.setup(ev.name, ev.option);
				});

				bool cont = true;
				chn_worker.listen<event_stop>([&](auto) {
					log << "(worker) stop request\n";
					cont = false;
				});

				while (cont) chn_worker.wait();
			}};

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

		void setup(filename_type f, option_type o)
		{
			chn_worker.post(event_setup{std::move(f), std::move(o)});
		}

		void stop()
		{
			chn_worker.post(event_stop{});
			if (th.joinable()) th.join();
			log << "shutdown.\n\n";
		}
	}
}

