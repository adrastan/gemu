#ifndef LCD_CONTROLLER
#define LCD_CONTROLLER

#define MODE_0 204
#define MODE_2 80
#define MODE_3 172
#define TOTAL 456

#include "system_types.h"
#include "memory.h"
#include "interrupt.h"
#include <SDL.h>

typedef struct {
    int index;
    int auto_inc;
    u8 palette[64];
} Palette;

struct Pixel {
    int red;
    int green;
    int blue;
};

struct Tile {
    struct Pixel px;
    u8 attributes;
    int priority;
    int v_flip;
    int h_flip;
    int bank;
    int palette;
    int colour_number;
    u8 byte1;
    u8 byte2;
    int transparent;
};

struct Sprite {
    struct Pixel px;
    u8 y_pos;
    u8 x_pos;
    u8 tile_number;
    u8 attributes;
    int bank;
    int palette;
    int priority;
    int colour_number;
    u16 address;
    u8 byte1;
    u8 byte2;
    int transparent;
};

class Memory;
class Cartridge;
class Interrupt;

class LCDController
{
    public:
    LCDController(Memory * const);
    int fps_count = 0;
    int interrupt_cycles[4] = {MODE_0, MODE_2, MODE_3, TOTAL};
    Palette bg_palette, spr_palette;
    Memory *memory;
    Cartridge *cart;
    SDL_Surface* screen_surface;
    SDL_Renderer* renderer;
    Interrupt* interrupt;

    u8 screen[144][160][3] = {};

    void draw_frame();
    void update(int);
    void switch_mode(int);
    int get_mode();
    void update_graphics();
    void update_pixel(int);
    void get_pixel(int);
    void draw_pixel(int, int, int, int);
    struct Tile get_tile(int);
    struct Sprite get_sprite(int);
    struct Pixel get_colour(int, int, Palette *, u8);
    void sort_sprites(struct Sprite spr[], int);
    u16 get_sprite_address(int sprite_size, struct Sprite *spr);
    void compare_ly();
    void inc_ly();
};

#endif //LCD_CONTROLLER
