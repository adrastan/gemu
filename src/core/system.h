#ifndef SYSTEM_H
#define SYSTEM_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>
#include <memory>

#include "cartridge.h"
#include "file.h"
#include "lcd_controller.h"
#include "system_types.h"
#include "memory.h"
#include "cpu.h"
#include "sound.h"
#include "timers.h"

#define SCREEN_HEIGHT 300
#define SCREEN_WIDTH 300

class System
{
    public:
    System(const std::string);
    ~System();

    static constexpr int op_cycles[256] = {4,12,8,8,4,4,8,4,20,8,8,8,4,4,8,4,
                     4,12,8,8,4,4,8,4,12,8,8,8,4,4,8,4,
                     12,12,8,8,4,4,8,4,12,8,8,8,4,4,8,4,
                     12,12,8,8,12,12,12,4,12,8,8,8,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     8,8,8,8,8,8,4,8,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     20,12,16,16,24,16,8,16,20,16,16,4,24,24,8,16,
                     20,12,16,0,24,16,8,16,20,16,16,0,24,0,8,16,
                     12,12,8,0,0,16,8,16,16,4,16,0,0,0,8,16,
                     12,12,8,4,0,16,8,16,12,8,16,4,0,0,8,16};
    static constexpr int pre_cycles[256] = {8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8};

    bool is_running = false;
    bool halt = false;
    int fps_count = 0;
    int counter = 0;
    int delay = 0;

    u8 opcode;

    SDL_Window* sdl_window = NULL;
    SDL_Surface* screen_surface = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    Cartridge *cart;
    std::unique_ptr<File> rom_file;
    std::unique_ptr<LCDController> lcd_controller;
    std::unique_ptr<Sound> sound_controller;
    std::unique_ptr<Memory> memory;
    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Timers> timers;

    void power_on();
    void power_off();
    void init_SDL();
    void stop_SDL();
    void load_cart_from_file(const std::string);
    void draw_frames();
    void poll_events();
    void next_op();
    void process_opcode();
    void check_interrupts();
};

#endif //SYSTEM_H
