// TapCombiner.cpp : Defines the entry point for the console application.
//
#include "TapCombiner.h"


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Wrong parameters number! Please enter the folder that contains *.tap files. E.g. \"D:/folder\" " << std::endl;
	
		return 1;
	}

	try {

		TapCombiner tapCombiner{ std::string(argv[1]) };
		if (tapCombiner.exec())
		{
			std::cout << "We've done well. Go check it!" << std::endl;
		}
	}

	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Something has gone really wrong." << std::endl;
		return 1;
	}

    return 0;
}

