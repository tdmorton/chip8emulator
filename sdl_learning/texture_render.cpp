/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <chrono>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		//if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		//{
		//	printf( "Warning: Linear texture filtering not enabled!" );
		//}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
				gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
				
			uint32_t pixels[2048];
			uint8_t gfx[2048];
			
			for (int i = 0; i < 2048; ++i)
			{
				gfx[i] = 0x00;
			}
			
			gfx[0] = 0xFF;
			gfx[2] = 0xFF;
			gfx[3] = 0xFF;
			gfx[6] = 0xFF;
			gfx[7] = 0xFF;
			gfx[71] = 0xFF;
			gfx[135] = 0xFF;
			gfx[136] = 0xFF;
			gfx[137] = 0xFF;
			gfx[138] = 0xFF;
			
			
				
				// Store pixels in temporary buffer
            for (int i = 0; i < 2048; ++i) {
                uint8_t pixel = gfx[i];
                pixels[i] = (pixel & 0xFFFFFFFF)| (pixel<<8 & 0xFFFFFFFF) | (pixel<<16 & 0xFFFFFFFF) | 0xFF000000;
            }
            // Update SDL texture
            SDL_UpdateTexture(gTexture, NULL, pixels, 64 * sizeof(Uint32));
            // Clear screen and render
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
            SDL_RenderPresent(gRenderer);

			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

				//Update screen
				SDL_RenderPresent( gRenderer );
				std::this_thread::sleep_for(std::chrono::microseconds(1200));
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
