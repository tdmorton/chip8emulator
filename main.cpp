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
	
	std::chrono::milliseconds frame_duration(1000 / 60); // 16.67 milliseconds for 60 FPS
	
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
	
	if (memLoadedGood)
	{
		std::cout << "Memory does not match ROM" << std::endl;
		return 1;
	}
	
	int counter = 0;
	
	while (!timeToQuit)
	{
		std::chrono::steady_clock::time_point frame_start = std::chrono::steady_clock::now();
		ch8.emulateOneCycle();
		if (ch8.drawFlag)
		{
			myScreen.drawScreen(ch8.screen);
			ch8.drawFlag = false;
		}
		++counter;
		timeToQuit = myScreen.readKeys();
		std::chrono::steady_clock::time_point frame_end = std::chrono::steady_clock::now();
        std::chrono::milliseconds elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);
		if (elapsed_time < frame_duration) 
		{
            std::this_thread::sleep_for(frame_duration - elapsed_time);
		}
	}

	//std::cout << SDLK_ESCAPE << std::endl;
	
	return 0;
}
