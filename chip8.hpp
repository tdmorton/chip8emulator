#ifndef chip8_h
#define chip8_h

#include <cstdint>

class chip8
{
	public:
	uint16_t pc;	// program counter
	uint16_t sc;	// stack counter
	uint8_t vreg[16]; // 16 8-bit registers
	uint16_t stack[16]; // 16 16-bit stack data
	uint16_t index; 	// memory index
	uint8_t memory[4096]; // memory
	
	uint8_t fonts[80]; 
	
};
	
#endif