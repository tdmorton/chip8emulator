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
	memset(imBuf, 0, sizeof(imBuf));
	

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
		memory[i] = fonts[i-0x050];
	}
	
	return 0;
	
}

bool chip8::loadRom(char* filename)
{
	std::ifstream file(filename, std::ios::binary);		// grabs rom
	
	if (!file) 
	{
		std::cerr << "Could not open file " << filename << std::endl;
		return 1;
	}
		
	uint8_t tempRom;
	pc = 0x200;
		
	while(file.read(reinterpret_cast<char*>(&tempRom), sizeof(tempRom)))
	{
		memory[pc] = tempRom;
		++pc;
	}
		
	file.close();
	
	pc = 0x200; 			// reset pc
	
	return 0;
}

bool chip8::checkRom(char* filename)
{
	std::ifstream file(filename, std::ios::binary);		// grabs rom
	
	
	if (!file) 
	{
		std::cerr << "Could not open file " << filename << std::endl;
		return 1;
	}
		
	uint8_t tempRom;
	pc = 0x200;
		
	while(file.read(reinterpret_cast<char*>(&tempRom), sizeof(tempRom)))
	{
		if (memory[pc] != tempRom)
		{
			return 1;
		}
		++pc;
	}
		
	file.close();
	
	pc = 0x200; 			// reset pc
	
	return 0;
}

bool chip8::emulateOneCycle(screen myScreen, bool debugMode)
{

	if (pc < 0 || pc >= 4096)
	{
		std::cerr << "Program counter out of bounds; pc = " << pc << std::endl;
		return 1;
	}
	if (sp < 0 || sp > 15)
	{
		std::cerr << "Stack counter out of bounds; sp = " << sp << std::endl;
		return 1;
	}

	// grab next 2 byte opcode
	opcode = memory[pc] << 8 | memory[pc+1];
	
	// use switch statement, function pointer implementation maybe later
	
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	uint8_t N = opcode & 0x000F;
	uint8_t NN = opcode & 0x00FF;
	uint16_t NNN = opcode & 0x0FFF;

	if (debugMode)
	{
		std::cout << "Opcode: 0x" << std::hex << static_cast<int>((opcode & 0xF000) >> 12) << static_cast<int>(X) << static_cast<int>(Y) << static_cast<int>(N) << " PC = " << pc << std::endl;
	}
	
	// switch based on first hex number of opcode
	switch(opcode & 0xF000)
	{
		case 0x0000:
		{
			switch(opcode)
			{
				// clear screen
				case 0x00E0:
				{
					for (int i = 0; i < 64*32; ++i)
					{
						imBuf[i] = 0x00;
					}
					drawFlag = true;
					pc += 2;
					break;
				}
				// return from subroutine
				case 0x00EE:
				{
					--sp;
					pc = stack[sp];
					pc += 2;
					break;
				}
				default:
				{
					std::cout << "Unknown Opcode: " << std::hex << opcode  << " pc = " << pc << std::endl;
					pc += 2;
					break;
				}
			}
			break;
		}	
		// 0x1NNN Jump to NNN
		case 0x1000:
		{
			pc = NNN;
			break;
		}
		// 0x2NNN Call subroutine at NNN
		case 0x2000:
		{
			stack[sp] = pc;
			pc = NNN;
			++sp;
			break;
		}
		// 0x3XNN Skip next instruction based on condition
		case 0x3000:
		{
			if (V[X] == NN)
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		}
		// 0x4XNN Skip next instruction based on condition
		case 0x4000:
		{
			if (V[X] == NN)
			{
				pc += 2;
			}
			else
			{
				pc += 4;
			}
			break;
		}
		// 0x5XNN Skip next instruction based on condition
		case 0x5000:
		{
			if (V[X] == V[Y])
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		}
		// 0x6XNN Set VX to NN
		case 0x6000:
		{
			V[X] = (opcode & 0x00FF);
			pc += 2;
			break;
		}
		// 0x7XNN Add NN to VX
		case 0x7000:
		{
			V[X] += (opcode & 0x00FF);	
			pc += 2;
			break;
		}
		// 0x8XY0-0x8XYE
		case 0x8000:
		{
			switch (opcode & 0x000F)
			{
				// 0x8XY0 VX = VY
				case 0x0000:
				{
					V[X] = V[Y];
					pc += 2;
					break;
				}
				// 0x8XY1 VX = VX OR VY
				case 0x0001:
				{
					V[X] |= V[Y];
					pc += 2;
					break;
				}
				// 0x8XY2 VX = VX AND VY
				case 0x0002:
				{
					V[X] &= V[Y];
					pc += 2;
					break;
				}
				// 0x8XY3 VX = VX XOR VY	
				case 0x0003:
				{
					V[X] ^= V[Y];
					pc += 2;
					break;
				}
				// 0x8XY4 VX = VX + VY (VF carry set)
				case 0x0004:
				{
					uint8_t carry;
					if ((V[X] + V[Y]) > 0xFF)
					{
						carry = 1;
					}
					else
					{
						carry = 0;
					}
					V[X] += V[Y];
					V[0xF] = carry;
					pc += 2;
					break;
				}
				// 0x8XY5 VX = VX - VY (VF carry set)
				case 0x0005:
				{
					uint8_t carry;
					if (V[X] >= V[Y])
					{
						carry = 1;
					}
					else
					{
						carry = 0;
					}
					V[X] -= V[Y];
					V[0xF] = carry;
					pc += 2;
					break;
				}
				// 0x8XY6 Bitshift VX right by 1 (VF carry set)
				case 0x0006:
				{
					uint8_t carry;
					carry = (V[X] & 0x01);
					V[X] = V[X] >> 1;
					V[0xF] = carry;
					pc += 2;
					break;
				}
				// 0x8XY7 VX = VY - VX (VF carry set)
				case 0x0007:
				{
					uint8_t carry;
					if (V[Y] >= V[X])
					{
						carry = 1;
					}
					else
					{
						carry = 0;
					}
					V[X] = V[Y] - V[X];
					V[0xF] = carry;
					pc += 2;
					break;
				}
				// 0x8XYE Bitshift VX left by 1 (VF carry set)
				case 0x000E:
				{
					uint8_t carry;
					carry = V[X] >> 7;
					V[X] = V[X] << 1;
					V[0xF] = carry;
					pc += 2;
					break;
				}
				default:
				{
					std::cout << "Unknown Opcode: " << std::hex << opcode  << " pc = " << pc << std::endl;
					pc += 2;
					break;
				}
			}
			break;
		}
		// 0x9XY0 Skip next instruction based on condition
		case 0x9000:
		{
			if (V[X] != V[Y])
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		}
		// 0xANNN Set I to NNN
		case 0xA000:
		{
			I = NNN;
			pc += 2;
			break;
		}
		// 0xBNNN Jump to NNN
		case 0xB000:
		{
			//stack[sp] = pc;
			//++sp;				//stack does not need to be used
			pc = V[0] + NNN;
			pc += 2;
			break;
		}
		// 0xCXNN Set VX to a random number with NN bitmask
		case 0xC000:
		{
			V[X] = static_cast<uint8_t>(dist256(rng) & NN);
			pc += 2;
			break;
		}
		// 0xDXYN Draw sprite to screen
		case 0xD000:
		{
			// x coordinate of sprite
			int x = V[X];
			// y coordinate of sprite
			int y = V[Y];
			// number of bytes
			int n = N;
			// width (always 1 byte)
			int w = 8;
			// bit mask for pixels in byte
			uint8_t mask = 0x80;
			// VF is set to 1 if collision, 0 if not
			V[0xF] = 0;

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < w; j++)
				{
					if ((memory[I+i] & mask >> j) != 0)
					{
						if (imBuf[(((y+i)*64) + (x+j))] == 1)
						{
							V[0x0F] = 1;
						}
						imBuf[((y+i)*64) + (x+j)] ^= 0x1;
					}
				}
			}
			drawFlag = true;
			pc += 2;
			break;
		}
		// 0xE??? series of opcodes
		case 0xE000:
		{
			switch(opcode & 0x00F0)
			{
				// 0xEX9E Skip next instruction if key in VX is pressed
				case 0x0090:
				{
					if (myScreen.keysPressed[V[X]])
					{
						pc += 4;
					}
					else
					{
						pc += 2;
					}
					break;
				}
				// 0xEX9E Skip next instruction if key in VX is not pressed
				case 0x00A0:
				{
					if (!myScreen.keysPressed[V[X]])
					{
						pc += 4;
					}
					else
					{
						pc += 2;
					}
					break;
				}
			}
			break;
		}
		case 0xF000:
		{
			switch(opcode & 0x000F)
			{
				// 0xFX33 Store BCD VX at I
				case 0x0003:
				{
					memory[I] = static_cast<uint8_t>(V[X]/100); // hundreds place
					memory[I+1] = static_cast<uint8_t>((V[X]/10)%10); // tens place
					memory[I+2] = static_cast<uint8_t>(V[X]%10); // ones place
					pc += 2;
					break;
				}
				case 0x0005:
				{
					switch(opcode & 0x00F0)
					{
						// 0xFX15 Set the delay timer to VX
						case 0x0010:
						{
							delayTimer = V[X];
							pc += 2;
							break;
						}
						// 0xFX55 Store V0 - VX to memory starting at I
						case 0x0050:
						{
							for (int i = 0; i <= X; ++i)
							{
								memory[I+i] = V[i];
							}
							I = I + X + 1;
							pc += 2;
							break;
						}
						// 0xF065 Set V0 - VX from memory starting at I
						case 0x0060:
						{
							for (int i = 0; i <= X; ++i)
							{
								V[i] = memory[I+i];
							}
							// I needs to be set to end of regs
							I = I + X + 1;
							pc += 2;
							break;
						}
					}
					break;
				}
				// FX07 Set VX to delay timer value
				case 0x0007:
				{
					V[X] = delayTimer;
					pc += 2;
					break;
				}
				// FX18 Set sound delay to VX
				case 0x0008:
				{
					soundTimer = V[X];
					pc += 2;
					break;
				}
				// FX29 Set I to Font location in VX
				case 0x0009:
				{
					I = (V[X] * 5) + 0x50;
					pc +=2;
					break;
				}
				// FX0A Wait for key press and store in VX
				case 0x000A:
				{
					bool keyPressed = false;
					for (int i = 0; i < 16; i++)
					{
						if (myScreen.keysPressed[i])
						{
							V[X] = myScreen.chipKeys[i];
							keyPressed = true;
						}
					}
					if (keyPressed)
					{
						pc += 2;
						keyPressed = false;
					}
					break;
				}
				// FX1E Add VX to I (VF carry set)
				case 0x000E:
				{
					if (I+V[X] > 0x0FFF)
					{
						V[0xF] = 1; // may or may not be needed
					}
					I += V[X];
					pc += 2;
					break;
				}
				default:
				{
					std::cout << "Unknown Opcode: " << std::hex << opcode << std::endl;
					break;
				}
			}
			break;
		}
		default:
		{
			std::cout << "Unknown Opcode: " << std::hex << opcode << std::endl;
			pc += 2;
			break;
		}
	}

	if (delayTimer > 0)
	{
		delayTimer--;
	}
	if (soundTimer > 0)
	{
		soundTimer--;
	}

	return 0;
}
	
	
	
