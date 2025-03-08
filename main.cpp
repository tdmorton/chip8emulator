#include <cstdint>
#include <iostream>
#include <fstream>
#include "chip8.hpp"

int main(int argc, char** argv)
{
	
	chip8 ch8;
	
	bool weGood = ch8.init();
	
	std::cout << argc << std::endl;
	
	for (int i = 0; i < argc; ++i)
	{
		std::cout << i << ": " << argv[i] << std::endl;
	}
	
	bool gotAFile = ch8.loadRom(argv[1]);
	
	std::cout << "Hello World, is chip8 is initialized?: " << weGood << std::endl;
	
	std::cout << "0 means rom is good, 1 means rom is bad" << ch8.checkRom(argv[1]) << std::endl;
	
	int counter = 0;
	
	while (counter < 200)
	{
		ch8.emulateOneCycle();
	}
	
	return 0;
}
