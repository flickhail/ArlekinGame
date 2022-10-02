#include "Main.hpp"

int main()
{
	try
	{
		GameEngine::Run(950	, 600);
	}

	catch (const std::runtime_error& except)
	{
		std::cout << "Runtime exception has occured:\n" << except.what() << std::endl;
	}

	catch (const std::exception& except)
	{
		std::cout << "General exception has occured:\n" << except.what() << std::endl;
	}
	
	return 0;
}

