#include <SDL2/SDL.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cpu.h"

#define SCREEN_WIDTH 160*3
#define SCREEN_HEIGHT 144*3

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, char** args)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        //Initialize window and renderer
        window = SDL_CreateWindow("gbemu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            char * file_location = args[1];
            if (file_location == NULL) {
                puts("file not found");
                return 0;
            }

            // go to start of file name
            while (*file_location != '\0') {
                ++file_location;
            }
            while (*file_location != '\\') {
                --file_location;
            }
            ++file_location;

            file_name = file_location;
            char a[100];
            char * b = file_name;
            int i;

            // copy rom title to a
            for (i = 0; *b != '.'; ++i) {
                a[i] = *b;
                ++b;
            }
            a[i] = '\0';

            // create save file extension
            char c[100];
            strcpy(c,a);
            title = a;
            strcat(c,".SAVE");
            save_file = c;

            start_cpu();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
