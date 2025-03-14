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
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;	
	
	uint32_t pixels[2048];			//SDL expects 32 bit integers for ARGB8888
	
	//Starts up SDL and creates window
	bool init();
	
	//Frees media and shuts down SDL
	void close();
	
	//Draws the contents of pixelBuffer onto the screen
	bool drawScreen(uint8_t * pixelBuffer);

	//Reads the current key press events
	bool readKeys(uint8_t * keys);
	
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