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




		void run(command_func_type cmd)
		{
			// create worker thread
			std::thread th([cmd] {
				vimlight::vim vim(cmd);
				highlight::group group("hlgroup");
				vimlight::analyzer analyzer;
				highlight::delta delta;
				init::done();

				// main loop
				while (true) {
					auto src = update::wait();
					auto result = analyzer.parse(src, group);
					delta.update(result, vim);
				}
			});
			th.detach();

			// wait for initialization done
			init::wait();
		}

		void request(source_type src)
		{
			update::request(std::move(src));
		}
	};
};


