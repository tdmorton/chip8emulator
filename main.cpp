#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include "chip8.hpp"
#include "screen.hpp"
#include <chrono>
#include <thread>

bool validateFileName(char*);
void printHelp();

int main(int argc, char** argv)
{
	// in opcodes/second, not exactly a framerate
	int executionSpeed = 360;
	
	if (argc < 2)
	{
		std::cerr << "Please include a name for the ROM to load. Type -h for details." << std::endl;
		return 1;				// exit main
	}
	
	bool debugMode = false;
	char* filename;
	
	if (!strcmp(argv[1], "-h"))
	{
		printHelp();
		return 0;
	}
	else if (!validateFileName(argv[1]))
	{
		filename = argv[1];
	}
	else if (!strcmp(argv[1], "-d") && argc > 2)
	{
		debugMode = true;
		if (!validateFileName(argv[2]))
		{
			filename = argv[2];
		}
	}
	else
	{
		std::cout << "Invalid argument(s), use -h to see the correct usage" << std::endl;
		return 1;			
	}
	
	std::chrono::milliseconds frame_duration(1000 / executionSpeed);

	bool timeToQuit = 0;

	screen myScreen;

	chip8 ch8;
	
	bool ch8Init = ch8.init();

	bool screenInit = myScreen.init();
	
	bool fileLoaded = ch8.loadRom(filename);
	
	bool memBad = ch8.checkRom(filename);
	
	if (memBad)
	{
		std::cout << "Memory does not match ROM" << std::endl;
		return 1;
	}
	
	int counter = 0;
	
	while (!timeToQuit)
	{
		std::chrono::steady_clock::time_point frame_start = std::chrono::steady_clock::now();
		ch8.emulateOneCycle(myScreen, debugMode);
		if (ch8.drawFlag)
		{
			myScreen.drawScreen(ch8.imBuf);
			ch8.drawFlag = false;
		}
		++counter;
		timeToQuit = myScreen.readKeys(debugMode);
		std::chrono::steady_clock::time_point frame_end = std::chrono::steady_clock::now();
        std::chrono::milliseconds elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);
		if (elapsed_time < frame_duration) 
		{
            std::this_thread::sleep_for(frame_duration - elapsed_time);
		}
	}
	
	return 0;
}

bool validateFileName(char* filename)
{
	int length = strlen(filename);
	if (length < 5)
	{
		return 1;
	}
	char fileExtension[4];
	for (int i = 0; i < 4; i++)
	{
		fileExtension[i] = filename[i+(length-4)];
	}
	if (!strcmp(fileExtension, ".ch8"))
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
void printHelp()
{
	std::cout << "usage: ./chip8 [-d] [-h] rom.ch8" << std::endl;
}
