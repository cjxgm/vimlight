#include "worker.hh"
#include "channel.hh"
#include "highlight/group.hh"
#include "log.hh"
#include <thread>
#include <utility>

namespace vimlight
{
	namespace worker
	{
		namespace
		{

			namespace events
			{
				struct done : event<3> {};

				struct request : event<2>
				{
					source_type src;
					request(source_type src) : src{std::move(src)} {}
				};

				struct result : event<1>
				{
					commands_type cmds;
					result(commands_type cmds) : cmds{std::move(cmds)} {}
				};

				struct setup : event<3>
				{
					filename_type name;
					option_type option;
					setup(filename_type name, option_type option)
						: name{std::move(name)}, option{std::move(option)} {}
				};

				struct stop : event<4> {};
			}

			channel<events::request, events::setup, events::stop> chn_worker;
			channel<events::done, events::result> chn_main;
			std::thread th;
		}



		struct worker_callback
		{
			bool operator () (events::stop) const
			{
				log << "(worker) stop request\n";
				return false;
			}

			bool operator () (events::request const& ev) const
			{
				log << "(worker) parse request\n";
				auto result = analyzer.parse(ev.src, group);
				chn_main.post<events::result>({vim::highlight(result)});
				return true;
			}

			bool operator () (events::setup const& ev) const
			{
				log << "(worker) setup request\n";
				log << "\t" << ev.name << '\n';
				log << "\t" << ev.option << '\n';
				analyzer.setup(ev.name, ev.option);
				return true;
			}

			worker_callback(
					vimlight::analyzer & analyzer,
					highlight::group & group)
				: analyzer{analyzer}
				, group{group} {}

		private:
			vimlight::analyzer & analyzer;
			highlight::group & group;
		};

		void start(filename_type const& hlgroup)
		{
			th = std::thread{[&hlgroup] {
				vimlight::analyzer analyzer;
				highlight::group group(hlgroup);
				chn_main.post<events::done>();

				while (chn_worker.listen<worker_callback>({analyzer, group})) {}
			}};

			chn_main.wait<events::done>();
		}

		void request(source_type src)
		{
			chn_worker.post<events::request>({std::move(src)});
		}

		bool done()
		{
			return chn_main.template is<events::result>();
		}

		commands_type get()
		{
			return chn_main.template get<events::result>().cmds;
		}

		void setup(filename_type f, option_type o)
		{
			chn_worker.post<events::setup>({std::move(f), std::move(o)});
		}

		void stop()
		{
			chn_worker.post<events::stop>();
			if (th.joinable()) th.join();
			log << "shutdown.\n\n";
		}
	}
}

