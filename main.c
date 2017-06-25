#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "cpu.h"

#define SCREEN_WIDTH 160*3
#define SCREEN_HEIGHT 144*3

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;

char a[100];
char c[100];

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
            if (args[1] == NULL) {
                puts("file not found");
                return 0;
            }
            prepare_file(args[1]);
            start_cpu();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void prepare_file(char * file_n)
{
    file_location = file_n;
    // go to start of file name
    while (*file_n != '\0') {
        ++file_n;
    }
    while (*file_n != '/') {
        --file_n;
    }
    ++file_n;

    char * b = file_n;
    int i;

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
