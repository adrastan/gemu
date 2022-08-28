#ifndef SYSTEM_H
#define SYSTEM_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

#define SCREEN_HEIGHT 300
#define SCREEN_WIDTH 300

class Cartridge;
class File;

class System
{
    public:
    System();
    ~System();

    bool is_running = false;
    SDL_Window* sdl_window = NULL;
    SDL_Surface* screen_surface = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;
    Cartridge *cart;
    File *rom_file;

    void power_on();
    void power_off();
    void init_SDL();
    void stop_SDL();
    void load_cart_from_file(const std::string);
};

#endif //SYSTEM_H