#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#define SCREEN_HEIGHT 300
#define SCREEN_WIDTH 300

class System
{
    public:
    bool is_running = false;
    SDL_Window* sdl_window = NULL;
    SDL_Surface* screen_surface = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    void power_on();
    void power_off();
    void init_SDL();
    void stop_SDL();
};