#include "screen.hpp"

bool screen::init()
{
	//Initialization flag
	bool success = true;


	uint8_t tempKeys[16] = 
	{
		SDLK_x,
		SDLK_1,
		SDLK_2,
		SDLK_3,
		SDLK_q,
		SDLK_w,
		SDLK_e,
		SDLK_a,
		SDLK_s,
		SDLK_d,
		SDLK_z,
		SDLK_c,
		SDLK_4,
		SDLK_r,
		SDLK_f,
		SDLK_v
	};

	for (int i = 0; i < sizeof(chipKeys); ++i)
	{
		chipKeys[i] = tempKeys[i];
	}

	memset(keysPressed, 0, sizeof(keysPressed));

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{

		//Create window
		gWindow = SDL_CreateWindow( "Chip 8 Interpreter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
		pixels[i] = (pixel * 0x00FFFFFF) | 0xFF000000;
	}
	// Update SDL texture
	SDL_UpdateTexture(gTexture, NULL, pixels, 64 * sizeof(Uint32));
	// Clear screen and render
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	return 0;
}

bool screen::readKeys(bool debugMode)
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
				else
				{
					//std::cout << "pressed a key" << std::endl;
					for (int i = 0; i < sizeof(chipKeys); ++i)
					{
						//std::cout << "Checking " << chipKeys[i] << " key" << std::endl;
						if (e.key.keysym.sym == chipKeys[i])
						{
							keysPressed[i] = 1;
							if (debugMode)
							{
								std::cout << "Set key " << chipKeys[i] << " to pressed." << std::endl;
							}
						}
					}
				}
			}
			else if (e.type == SDL_KEYUP)
			{
				for (int i = 0; i < sizeof(chipKeys); ++i)
				{
					if (e.key.keysym.sym == chipKeys[i])
					{
						keysPressed[i] = 0;
						if (debugMode)
						{
							std::cout << "Set key " << chipKeys[i] << " to unpressed." << std::endl;
						}
					}
				}
			}
		}
	}
	return 0;
}