#include "worker.hh"
#include <iostream>
using std::cout;
using std::endl;

namespace vimlight
{
	namespace worker
	{
		void run(command_func_type cmd)
		{
			cout << ">>> worker::run " << reinterpret_cast<void*>(cmd) << endl;
		}

		void request(const source_type& src)
		{
			cout << ">>> worker::request <<" << endl
				<< src << endl
				<< ">>" << endl;
		}
	};
};

