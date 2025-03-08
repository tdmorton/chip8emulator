#include <cstdint>
#include <iostream>
#include <fstream>
#include "chip8.hpp"

int main()
{
	
	chip8 ch8;
	
	bool weGood = ch8.init();
	
	bool gotAFile = ch8.loadRom();
	
	std::cout << "Hello World, is chip8 is initialized?: " << weGood << std::endl;
	
	std::cout << "0 means rom is good, 1 means rom is bad" << ch8.checkRom() << std::endl;
	
	return 0;
}
