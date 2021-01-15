// All the inspector does is print out the arguments passed to it via the shell and then wait.

#include <iostream>

int main(int argc, const char ** argv)
{
	for (int i = 0; i < argc; ++i)
	{
		std::cout << "\"" << argv[i] << "\"";
	}
	getchar();
}
