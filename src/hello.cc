#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;

namespace argument
{
	using argument_type = std::string;
	using list_type = std::vector<argument_type>;
	list_type list(int argc, char* argv[])
	{
		list_type lst(argc);
		for (int i=0; i<argc; i++)
			lst[i] = argument_type(argv[i]);
		return lst;
	}
};

int main(int argc, char* argv[])
{
	auto args = argument::list(argc, argv);
	for (auto& arg: args)
		cout << arg << endl;
	std::vector<int> x{1, 2, 3};
	return 0;
}

