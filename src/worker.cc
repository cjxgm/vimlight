#include "worker.hh"
#include "highlight/all.hh"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>

namespace vimlight
{
	namespace worker
	{
		// a communication channel with worker thread
		// this is used to tell the worker thread to update
		// the highlight. this will pass the last updated
		// source code to the worker thread.
		namespace update
		{
			using mutex_type = std::mutex;
			using condv_type = std::condition_variable;
			using  lock_type = std::unique_lock<mutex_type>;
			static mutex_type m;
			static condv_type cv;
			static bool requested = false;
			static source_type src;

			static source_type wait()
			{
				lock_type lock(m);
				cv.wait(lock, [] { return requested; });
				requested = false;
				return std::move(src);
			}

			static void request(source_type s)
			{
				{
					lock_type lock(m);
					src = std::move(s);
					requested = true;
				}
				cv.notify_one();
			}
		};


		// a communication channel with worker thread
		// this is used to tell if the worker thread is
		// initialized and ready to accept update requests.
		namespace init
		{
			using mutex_type = update::mutex_type;
			using condv_type = update::condv_type;
			using  lock_type = update::lock_type;
			static update::mutex_type m;
			static update::condv_type cv;
			static bool is_done = false;

			static void wait()
			{
				lock_type lock(m);
				cv.wait(lock, [] { return is_done; });
			}

			static void done()
			{
				{
					lock_type lock(m);
					is_done = true;
				}
				cv.notify_one();
			}
		};


		// a communication channel with worker thread
		// this is used to tell if the worker thread is
		// finished and is used for result-passing.
		namespace result
		{
			using mutex_type = update::mutex_type;
			using  lock_type = update::lock_type;
			static update::mutex_type m;
			static bool is_done = false;
			static commands_type commands;

			static void update(commands_type cmds)
			{
				lock_type lock(m);
				commands = std::move(cmds);
				is_done  = true;
			}

			static bool done()
			{
				lock_type lock(m);
				return is_done;
			}

			// should be called only when done() == true
			static commands_type get()
			{
				lock_type lock(m);
				is_done = false;
				return std::move(commands);
			}
		};




		void start(const filename_type& hlgroup)
		{
			std::thread th([&hlgroup] {
				vimlight::vim vim;
				vimlight::analyzer analyzer;
				highlight::group group(hlgroup);
				highlight::delta delta;
				init::done();

				while (true) {
					auto src = update::wait();
					auto result = analyzer.parse(src, group);
					delta.update(result, vim);
					result::update(std::move(vim.get()));
				}
			});
			th.detach();

			init::wait();
		}

		void request(source_type src)
		{
			update::request(std::move(src));
		}

		bool done()
		{
			return result::done();
		}

		commands_type get()
		{
			return std::move(result::get());
		}
	};
};

