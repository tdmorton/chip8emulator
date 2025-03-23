#include <cstdint>
#include <iostream>
#include <fstream>
#include "chip8.hpp"
#include "screen.hpp"
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
	
	if (argc < 2)
	{
		std::cerr << "Please include a name for the ROM to load. Type --help for details." << std::endl;
		return 1;				// exit main
	}
	
	chip8 ch8;

	bool timeToQuit = 0;

	screen myScreen;
	
	bool weGood = ch8.init();

	bool screenGood = myScreen.init();
	
	//std::cout << argc << std::endl;

	//std::cout << "chipKey[5]: " << myScreen.chipKeys[5] << std::endl;
	
	for (int i = 0; i < argc; ++i)
	{
		//std::cout << i << ": " << argv[i] << std::endl;
	}
	
	bool gotAFile = ch8.loadRom(argv[1]);
	
	bool memLoadedGood = ch8.checkRom(argv[1]);
	
	int counter = 0;
	
	while (counter < 30)
	{
		ch8.emulateOneCycle(myScreen.drawFlag);
		if (myScreen.drawFlag)
		{
			myScreen.drawScreen(ch8.screen);
		}
		++counter;
	}

	while (!timeToQuit)
	{
		timeToQuit = myScreen.readKeys();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::cout << SDLK_ESCAPE << std::endl;
	
	return 0;
}
