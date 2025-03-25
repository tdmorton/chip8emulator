#include "chip8.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist256(0,255);

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
		if (!file) 
		{
			std::cerr << "Could not open file " << filename << std::endl;
			return 1;
		}
		
		uint8_t tempRom;
		
		while(file.read(reinterpret_cast<char*>(&tempRom), sizeof(tempRom)))
		{
			memory[pc] = tempRom;
			//std::cout << std::hex << "Addr: " << pc-0x200 << "  ROM: " << static_cast<int>(tempRom) << "  Memory: " << static_cast<int>(memory[pc]) << std::endl;
			++pc;
		}
		
		file.close();
	
	pc = 0x200; 			// reset pc
	
	return 0;
}

bool chip8::checkRom(char* filename)
{
	std::ifstream file(filename, std::ios::binary);		// grabs rom
	
	//try
	//{
		if (!file) 
		{
			std::cerr << "Cannot find file " << filename << std::endl;
			return 1;//throw std::runtime_error("Could not open file");
		}
		
		uint8_t tempRom;
		pc = 0x200;
		
		while(tempRom = file.get())
		{
			//std::cout << std::hex << "ROM: " << static_cast<int>(tempRom) << "  Memory: " << static_cast<int>(memory[pc]) << std::endl;
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

bool chip8::emulateOneCycle(bool drawFlag)
{

	if (pc <= 0 || pc >= 4096)
	{
		std::cerr << "Program counter out of bounds" << std::endl;
		return 1;
	}
	if (sp <= 0 || sp >= 15)
	{
		std::cerr << "Stack counter out of bounds" << std::endl;
		return 1;
	}

	//std::cout << "Emulation Cycle: " << std::hex << pc << std::endl;
	opcode = memory[pc] << 8 | memory[pc+1];	// grab next opcode
	//pc += 2;									// increment pc by 2 (opcodes are 2 bytes)
	
	// use switch statement, function pointer implementation maybe later
	
	switch(opcode & 0xF000)						// bit mask, narrow switch statement to only 0-F
	{
		case 0x0000:
		{
			switch(opcode)
			{
				case 0x00E0:
				{
					for (int i = 0; i < 64*32; ++i)
					{
						screen[i] = 0x00;
					}
					drawFlag = true;
					pc += 2;
					break;
				}
				case 0x00EE:
				{
					--sp;
					pc = stack[sp];
					pc += 2;
					break;
				}
				default:
				{
					std::cout << "Unknown Opcode: " << std::hex << opcode << std::endl;
					break;
				}
			}
		}	
		case 0x1000:													// 0x1NNN
		{
			pc = opcode & 0x0FFF;										// opcode = NNN
			break;
		}
		case 0x2000:													// 0x2NNN
		{
			stack[sp] = pc;
			pc = opcode & 0x0FFF;
			++sp;
			break;
		}
		case 0x3000:													// 0x3XNN
		{
			if (V[(opcode & 0x0F00) >> 8] & (opcode & 0x00FF))			// if VX == NN
			{
				pc += 4;												// skip next instruction
			}
			else
			{
				pc += 2;
			}
			break;
		}
		case 0x4000:													// 0x4XNN
		{
			if (V[(opcode & 0x0F00) >> 8] & (opcode & 0x00FF))
			{
				pc += 2;
			}		// if VX != NN
			else
			{
				pc += 4;												// skip next instruction
			}
			break;
		}
		case 0x5000:													// 0x5XY0
		{
			if (V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4])	// if VX == VY
			{
				pc += 4;												// skip next instruction
			}
			else
			{
				pc += 2;
			}
			break;
		}
		case 0x6000:													// 0x6XNN
		{
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);				// VX = NN
			pc += 2;
			break;
		}
		case 0x7000:													// 0x7XNN
		{
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);				// VX += NN (no carry flag set)
			pc += 2;
			break;
		}
		case 0x8000:
		{
			switch (opcode & 0x000F)										// 0x8000-0x800E
			{
				case 0x0000:												// 0x8XY0
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];	// VX = VY
					pc += 2;
					break;
				}
				case 0x0001:												// 0x8XY1
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];	// VX |= VY
					pc += 2;
					break;
				}
				case 0x0002:												// 0x8XY2
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];	// VX &= VY
					break;
				}				
				case 0x0003:												// 0x8XY3
				{
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];	// VX ^= VY
					pc += 2;
					break;
				}
				case 0x0004:												// 0x8XY4
				{
					if ((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 0xFF)
					{
						V[0xF] = 1;
					}
					else
					{
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];								// VX += VY carry set
					pc += 2;
					break;
				}
				case 0x0005:												// 0x8XY5
				{
					if (V[(opcode & 0x0F00) >> 8] >= V[(opcode & 0x00F0) >> 4])							// VF = !underflow
					{
						V[0xF] = 1;
					}
					else
					{
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];								// VX -= VY carry set
					pc += 2;
					break;
				}
				case 0x0006:												// 0x8XY6
				{
					V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x0001);										// VF = VX LSB
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;							// VX >>= 1
					pc += 2;
					break;
				}
				case 0x0007:												// 0x8XY7
				{
					if (V[(opcode & 0x00F0) >> 4] >= V[(opcode & 0x0F00) >> 8])							// VF = !underflow
					{
						V[0xF] = 1;
					}
					else
					{
						V[0xF] = 0;
					}
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];	// VX -= VY carry set
					pc += 2;
					break;
				}
				case 0x000E:												// 0x8XYE
				{
					V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x8000);										// VF = VX MSB
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;							// VX <<= 1
					pc += 2;
					break;
				}
			}
		}
		case 0x9000:													// 0x9XY0
		{
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			{
				pc += 4;												// skip next instruction
			}
			else
			{
				pc += 2;
			}
			break;
		}
		case 0xA000:
		{
			I = (opcode & 0x0FFF) >> 4;
			pc += 2;
			break;
		}
		case 0xB000:
		{
			//stack[sp] = pc;
			//++sp;				//stack does not need to be used
			pc = V[0] + ((opcode & 0x0FFF) >> 4);
			pc += 2;
			break;
		}
		case 0xC000:
		{
			V[(opcode & 0x0F00) >> 8] = static_cast<uint8_t>(dist256(rng) & (opcode & 0x00FF));
			pc += 2;
			break;
		}
		case 0xD000:
		{
			//drawSprite(V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], V[(opcode & 0x000F)]);
			// x coordinate of sprite
			int x = V[(opcode && 0x0F00)>>8];
			// y coordinate of sprite
			int y = V[(opcode && 0x00F0)>>4];
			// number of bytes
			int n = V[opcode && 0x000F];
			// width (always 1 byte)
			int w = 8;
			// VF is set to 1 if collision, 0 if not
			V[0xF] = 0;

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < w; j++)
				{
					screen[((y*64) + x) + (8*i)] = memory[I+i];
				}
				
			}
			drawFlag = true;
			pc += 2;
			break;
		}
		case 0xE000:
		{
			switch(opcode & 0x00F0)
			{
				case 0x0090:
				{
					break;
				}
				case 0x00A0:
				{
					break;
				}
			}
		}
		default:
		{
			std::cout << "Unknown Opcode: " << std::hex << opcode << std::endl;
			break;
		}
	}
	return 0;
}
	
	
	