#include <string>
#include "system.h"
#include "cartridge.h"
#include "logger.h"
#include "file.h"

System::System()
{
    this->cart = NULL;
    this->rom_file = NULL;
}

System::~System()
{
    Logger::log("Destructing System...");
    delete this->cart;
    delete this->rom_file;
}

void System::power_on()
{
    if (this->cart == NULL) {
        Logger::log("Could not power on. Cart not initialised.");
        return;
    }

    this->init_SDL();
    this->is_running = true;

    Logger::log("Starting main game loop.");
    while (this->is_running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                // case SDL_KEYDOWN:
                // case SDL_KEYUP: update_joypad(&event->key); break;
                case SDL_QUIT:
                    this->power_off();
                    break;
            }
        }
    }
}

void System::power_off()
{
    Logger::log("Powering off.");
    this->is_running = false;
}

void System::load_cart_from_file(const std::string file_path)
{
    this->rom_file = new File(file_path);

    if (this->rom_file->buf == NULL)
    {
        Logger::log("Can't open rom file.");
        return;
    }

    this->cart = new Cartridge(this->rom_file->buf, this->rom_file->size);
    this->cart->print();
}

void System::init_SDL() {
    Logger::log("Initialising SDL..");

    //Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string s = SDL_GetError();
        Logger::log("SDL could not initialize! SDL_Error: " + s);
        return;
    }

    //Initialise SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::string s = SDL_GetError();
        Logger::log("SDL could not initialize! SDL_Error: " + s);
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
