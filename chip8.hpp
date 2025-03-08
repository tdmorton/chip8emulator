#ifndef chip8_h
#define chip8_h

#include <cstdint>
#include <string>

class chip8
{
	public:
	uint16_t pc;		// program counter
	uint16_t I; 		// memory address index
	uint16_t opcode;	// current opcode pulled from memory
	
	uint8_t memory[4096]; // memory
	uint8_t V[16]; 		// 16 8-bit registers
	
	uint16_t sp;		// stack pointer
	uint16_t stack[16]; // 16 16-bit stack data
	
	uint8_t screen[64*32]; // 64x32 pixel screen buffer
	
	uint16_t delayTimer;	// 16 bit delay timer
	uint16_t soundTimer;	// 16 bit sound timer
	
	uint8_t key[16];		// keypad (0-F)

	bool init();		// initialize emulator
	
	bool loadRom();	// loads rom from pc memory
	bool checkRom(); // checks memory to make sure it matches rom
	
};
	
#endif