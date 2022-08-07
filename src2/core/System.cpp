#include <iostream>
#include "System.h"

void System::power_on()
{
    this->init_SDL();
    this->is_running = true;
    
    while (this->is_running)
    {
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                // case SDL_KEYDOWN:
                // case SDL_KEYUP: update_joypad(&event->key); break;
                case SDL_QUIT: this->is_running = false; break;
            }
        }
    }
}

void System::power_off()
{
    this->is_running = false;
}

void System::init_SDL() {
    std::cout << "Initialising SDL.." << std::endl;
	
	//Initialize SDL Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	//Initialise SDL Audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl_window, &renderer);
    this->screen_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
}

void System::stop_SDL() 
{
    SDL_DestroyWindow(this->sdl_window);
	SDL_Quit();
}
