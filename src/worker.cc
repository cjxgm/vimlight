#include "worker.hh"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>

#include <iostream>
using std::cout;
using std::endl;

namespace vimlight
{
	namespace worker
	{
		namespace update
		{
			using mutex_type = std::mutex;
			using condv_type = std::condition_variable;
			using  lock_type = std::unique_lock<mutex_type>;
			static mutex_type m;
			static condv_type cv;
			static bool request = false;
			static source_type src;
		};

		void run(command_func_type cmd)
		{
			cout << ">>> worker::run " << reinterpret_cast<void*>(cmd) << endl;

			std::thread th([cmd] {
				source_type src;
				while (true) {
					{
						update::lock_type lock(update::m);
						update::cv.wait(lock, [] { return update::request; });
						src = std::move(update::src);
						update::request = false;
					}
					cout << ">>> worker::thread <<" << endl
						<< src << endl
						<< ">>" << endl;
				}
			});

			th.detach();
		}

		void request(source_type src)
		{
			cout << ">>> worker::request <<" << endl
				<< src << endl
				<< ">>" << endl;

			{
				update::lock_type lock(update::m);
				update::src = std::move(src);
				update::request = true;
			}
			update::cv.notify_one();
		}
	};
};


