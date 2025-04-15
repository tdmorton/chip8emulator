#ifndef screen_h
#define screen_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <chrono>
#include <cstdint>
#include <iostream>

class screen
{
	public:

	//map of all the keys to use
	uint8_t chipKeys[16] ;

	//map of if that key is currently pressed
	int keysPressed[16];

	//Draws the contents of pixelBuffer onto the screen
	bool drawScreen(uint8_t * pixelBuffer);

	//Reads the current key press events
	bool readKeys(bool debugMode);

	screen();
	~screen();

	private:

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 320;	
	

	//Pixel Buffer (SDL expects 32 bit integers for ARGB8888)
	uint32_t pixels[2048];			
	
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Current displayed texture
	SDL_Texture* gTexture = NULL;

	//Event handler
	SDL_Event e;
};

#endif