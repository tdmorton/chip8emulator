#include <cstdint>
#include <iostream>
#include <fstream>
#include "chip8.hpp"
#include "screen.hpp"

int main(int argc, char** argv)
{
	
	if (argc < 2)
	{
		std::cerr << "Please include a name for the ROM to load. Type --help for details." << std::endl;
		return 1;				// exit main
	}
	
	chip8 ch8;
	
	bool weGood = ch8.init();
	
	std::cout << argc << std::endl;
	

	
	
	for (int i = 0; i < argc; ++i)
	{
		std::cout << i << ": " << argv[i] << std::endl;
	}
	
	bool gotAFile = ch8.loadRom(argv[1]);
	
	bool memLoadedGood = ch8.checkRom(argv[1]);
	
	int counter = 0;
	
	while (counter < 15)
	{
		ch8.emulateOneCycle();
		++counter;
	}
	
	return 0;
}
