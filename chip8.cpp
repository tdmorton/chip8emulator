#include "chip8.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>

bool chip8::init()
{
	
	sp = 0;
	pc = 0x200;
	I = 0;
	
	memset(V, 0, sizeof(V));
	memset(memory, 0, sizeof(memory));
	memset(stack, 0, sizeof(stack));
	memset(screen, 0, sizeof(screen));
	

	uint8_t fonts[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	for (int i = 0x050; i < 0x0A0; i++)	// fontset is stored from 0x050 to 0x0A0
	{
		memory[i] = fonts[i];
	}
	
	return 0;
	
}

bool chip8::loadRom(char* filename)
{
	std::ifstream file(filename, std::ios::binary);		// grabs rom
	
	//try
	//{
		if (!file.is_open()) 
		{
			//throw std::runtime_error("Could not open file");
		}
		
		while(memory[pc] = file.get())
		{
			++pc;
		}
		
		if (file.bad())
		{
		//	throw std::runtime_error("Error occurred during reading file");
		}
		
		file.close();
	//}
	//catch(std::ifstream badFile)
	//{
		
	//	return 1;
		
	//}
	
	pc = 0x200; 			// reset pc
	
	return 0;
}

bool chip8::checkRom(char* filename)
{
	std::ifstream file("test_opcode.ch8", std::ios::binary);		// grabs rom
	
	//try
	//{
		if (!file.is_open()) 
		{
			//throw std::runtime_error("Could not open file");
		}
		
		uint8_t tempRom;
		pc = 0x200;
		
		while(tempRom = file.get())
		{
			if (tempRom != memory[pc])
			{
				return 1;
			}
			++pc;
		}
		
		if (file.bad())
		{
		//	throw std::runtime_error("Error occurred during reading file");
		}
		
		file.close();
	//}
	//catch(std::ifstream badFile)
	//{
		
	//	return 1;
		
	//}
	
	pc = 0x200;
	
	return 0;
}

bool chip8::emulateOneCycle()
{
	opcode = memory[pc] << 8 | memory[pc+1];	// grab next opcode
	pc += 2;									// increment pc by 2 (opcodes are 2 bytes)
	
	// use switch statement, function pointer implementation maybe later
	
	switch(opcode & 0xF000)						// bit mask, narrow switch statement to only 0-F
	{
		case 0x6000:
		{
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			std::cout << pc << ": " << std::hex << opcode <<  " | V[5] = " << std::hex << static_cast<int>(V[5]) << std::endl;
			break;
		}
	}
	return 0;
}
	
	
	