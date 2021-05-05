/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif
#include "cpu.h"
#include "sound.h"
#ifdef __cplusplus
}
#endif

#define SCREEN_WIDTH 160*3
#define SCREEN_HEIGHT 144*3

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;

char a[100];
char c[100];

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
	start_cpu(NULL);
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

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    screen_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
}

void stop_SDL() 
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void prepare_file(char * file_n)
{
	file_location = file_n;
	int i;
	int found = 0;

	// go to start of file name
	while (*(file_n++) != '\0') {
		if (*file_n == '/' || *file_n == '\\') {
			found = 1;
		}
	}
	if (found) {
		while (*file_n != '/' && *file_n != '\\') {
			--file_n;
		}
		++file_n;
	} else {
		file_n = file_location;
	}

	char * b = file_n;

	// copy rom title to a
	for (i = 0; *b != '.'; ++i) {
		a[i] = *b;
		++b;
	}
	a[i] = '\0';

	// create save file extension
	strcpy(c,a);
	title = a;
	strcat(c,".SAVE");
	save_file = c;
}
