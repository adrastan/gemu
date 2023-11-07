/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

#include "Core/cpu.h"
#include "Core/sound.h"

void stop_SDL(void);
void init_SDL(void);
void start_emulator(char *);

int main(int argc, char** args)
{
	start_emulator(args[1]);
	return 0;
}

void start_emulator(char* game)
{
	if (game != NULL) {
		prepare_file(game);
	}
	
	init_SDL();
	start_cpu(game);
    init_sound();
	start_main_loop();
	stop_SDL();
}

void init_SDL() {
	printf("Initialising SDL..\n");
	
	//Initialize SDL Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		stop_SDL();
		return;
	}

	//Initialise SDL Audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		stop_SDL();
		return;
	}

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl_window, &renderer);
    screen_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
}

void stop_SDL() 
{
    SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}
