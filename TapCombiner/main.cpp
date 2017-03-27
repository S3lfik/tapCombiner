// TapCombiner.cpp : Defines the entry point for the console application.
//
#include "TapCombiner.h"


int main(int argc, char* argv[])
{
	//if (argc != 2)
	//{
	//	std::cerr << "Wrong parameters number! Please enter the folder that contains *.tap files. E.g. \"D:/folder\" " << std::endl;
	//
	//	std::getchar();
	//	return 1;
	//}

	TapCombiner tapCombiner{ "taps" };//std::string(argv[1])};
	tapCombiner.exec();
	int a = 5;

    return 0;
}

