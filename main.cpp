#include <cstdint>
#include <iostream>
#include <fstream>

class chip8
{
	public:
	uint16_t pc;	// program counter
	uint16_t sc;	// stack counter
	uint8_t vreg[16]; // 16 8-bit registers
	uint16_t stack[16]; // 16 16-bit stack data
};


int main()
{
	std::cout << "Hello Worldis" << std::endl;
	
	return 0;
}
