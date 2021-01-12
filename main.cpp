#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <sstream>
#include <string>
//#include <iostream>  
//#include <iomanip>  
//#include <stdio.h>

#include "CUfoAttack.h"

using namespace std;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Joystick* gGameController = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;

bool init()
{
	//Initialization flag
	bool success = true;



	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Load joystick
		gGameController = SDL_JoystickOpen(0);
		if (gGameController == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
		//Create window
		gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_RendererInfo info;
				SDL_GetRendererInfo(gRenderer, &info);

				printf("%s\n", info.name);

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderSetLogicalSize(gRenderer, 800, 600);


				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

			}
		}

		gFont = TTF_OpenFont("fonts/8bitoperator/8bitoperator.ttf", 32);
		if (gFont == NULL)
		{
			printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
			//success = false;
		}

		SDL_ShowCursor(0);
	}

	return success;
}



#undef main
int main()
{


	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		CUfoAttack *level = new CUfoAttack();

		CEngine *gEngine = new CEngine(gRenderer);

		level->InitGame(gEngine, gRenderer, gFont);

		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		while (!quit)
		{
			Uint8 jSelect, jStart;

			if (gGameController != NULL)
			{
				jSelect = SDL_JoystickGetButton(gGameController, 6);
				jStart = SDL_JoystickGetButton(gGameController, 7);

				//printf("%d %d\n", jStart, jSelect);
				if (jSelect == 1 && jStart == 1)
				{
					quit = true;
					break;
				}
			}

			// Check for special keys like shift and control.
			const Uint8 *keys = SDL_GetKeyboardState(NULL);

			//Handle events on queuxe
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				{
					level->KeyDown(e.key.keysym.sym);

					//Select surfaces based on key press
					switch (e.key.keysym.sym)
					{
					
					case SDLK_h:
						if (keys[SDL_SCANCODE_RCTRL] || keys[SDL_SCANCODE_LCTRL])
						{
							gEngine->ShowHitBox(true);
						}
						break;

					case SDLK_j:
						if (keys[SDL_SCANCODE_RCTRL] || keys[SDL_SCANCODE_LCTRL])
						{
							gEngine->ShowHitBox(false);
						}
						break;

					case SDLK_ESCAPE:
						quit = true;
						break;

					default:

						break;
					}
				}
				else if (e.type == SDL_KEYUP && e.key.repeat == 0)
				{
					level->KeyUp(e.key.keysym.sym);

					//Select surfaces based on key press
					switch (e.key.keysym.sym)
					{
					case SDLK_SPACE:
						level->StartGame();
						
						break;

					default:

						break;
					}
				}
				else if (e.type == SDL_JOYBUTTONDOWN)
				{
					switch (e.jbutton.button)
					{
					case 0:
					case 5:
						level->KeyDown(SDLK_RIGHT);
						break;
					case 2:
						level->KeyDown(SDLK_UP);
						break;
					case 3:
					case 4:
						level->KeyDown(SDLK_LEFT);
						break;
					case 7:
						if (level->IsGameOver() == true)
						{
							level->StartGame();
						}
						break;
					}
				}
				else if (e.type == SDL_JOYBUTTONUP)
				{
					switch (e.jbutton.button)
					{
					case 0:
					case 5:
						level->KeyUp(SDLK_RIGHT);
						break;
					case 2:
						level->KeyUp(SDLK_UP);
						break;
					case 3:
					case 4:
						level->KeyUp(SDLK_LEFT);
						break;
					}
				}
				else if (e.type == SDL_JOYAXISMOTION)
				{
					//Motion on controller 0
					if (e.jaxis.which == 0)
					{
						//X axis motion
						if (e.jaxis.axis == 0)
						{
							//Left of dead zone
							if (e.jaxis.value < -8000)
							{
								// Map joystick left to a keyboard stroke.
								level->KeyDown(SDLK_a);
							}
							//Right of dead zone
							else if (e.jaxis.value > 8000)
							{
								// Map jouystick right to keyboard stroke.
								level->KeyDown(SDLK_d);
							}
							else
							{
								// Zero out the joystick when centered.
								level->KeyUp(SDLK_a);
								level->KeyUp(SDLK_d);
							}
						}
						//Y axis motion
						else if (e.jaxis.axis == 1)
						{
							//Below of dead zone
							if (e.jaxis.value < -8000)
							{
							}
							//Above of dead zone
							else if (e.jaxis.value > 8000)
							{
							}
							else
							{
							}
						}
					}
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					//Get mouse position
					int x, y;
					SDL_GetMouseState(&x, &y);
				}

			}

			level->Update();

			rand(); // Keep the random number generator unpredictable.

			SDL_RenderClear(gRenderer);

			gEngine->Render(); // Dump eveything to the screen.

			SDL_RenderPresent(gRenderer);

		}

		SDL_Quit();
	}
	return 0;
}

