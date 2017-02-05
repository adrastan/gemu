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
            start_cpu();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
