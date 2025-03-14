#include "screen.hpp"

bool screen::init()
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
			
			}
		}
	}

	return success;
}

void screen::close()
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

bool screen::drawScreen(uint8_t * pixelBuffer)
{
	// Store pixels in temporary buffer
	for (int i = 0; i < 2048; ++i) 
	{
		uint8_t pixel = pixelBuffer[i];
		pixels[i] = (pixel & 0xFFFFFFFF)| (pixel<<8 & 0xFFFFFFFF) | (pixel<<16 & 0xFFFFFFFF) | 0xFF000000;
	}
	// Update SDL texture
	SDL_UpdateTexture(gTexture, NULL, pixels, 64 * sizeof(Uint32));
	// Clear screen and render
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	return 0;
}

bool screen::readKeys(uint8_t * keys)
{
	//Handle events on queue
	while( SDL_PollEvent( &e ) != 0 )
	{
		//User requests quit
		if( e.type == SDL_QUIT)
		{
			return 1;
		}
		else
		{
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					return 1;
				}
				std::cout << "You pressed: " << e.key.keysym.sym << std::endl;
			}
			else if (e.type == SDL_KEYUP)
			{
				std::cout << "You released: " << e.key.keysym.sym << std::endl;
			}
		}
	}
	return 0;
}