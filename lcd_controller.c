#include <SDL2/SDL.h>
#include <stdio.h>
#include "cpu.h"
#include "memory.h"
#include "lcd_controller.h"

extern SDL_Window* window;
extern SDL_Surface* screen_surface;
extern SDL_Texture* texture;
extern SDL_Renderer* renderer;

u_int8 screen[160][144][3] = {};
u_int8 pixels[160*144*3];

void update_graphics()
{
    if (memory[0xff40] & 0x1) {
        draw_tiles();
    }
    if (memory[0xff40] & 0x2) {
        draw_sprites();
    }
}

void draw_tiles()
{
    u_int8 scrolly = memory[0xff42];
    u_int8 scrollx = memory[0xff43];
    u_int8 winy = memory[0xff4a];
    u_int8 winx = memory[0xff4b] - 7;
    int using_window = 0;
    int unsign = 1;
    u_int16 bg_map;
    u_int16 tile_data;
    u_int8 ly = memory[0xff44];

    if (memory[0xff40] & (1 << 5)) {
        if (winy <= ly) {
            using_window = 1;
        }
    }

    if (using_window) {
        if (memory[0xff40] & (1 << 6)) {
            bg_map = 0x9C00;
        } else {
            bg_map = 0x9800;
        }
    } else {
        if (memory[0xff40] & (1 << 3)) {
            bg_map = 0x9C00;
        } else {
            bg_map = 0x9800;
        }
    }

    if (memory[0xff40] & (1 << 4)) {
        tile_data = 0x8000;
    } else {
        tile_data = 0x8800;
        unsign = 0;
    }

    u_int8 y_pos, x_pos;

    if (!using_window) {
        y_pos = scrolly + ly;
    } else {
        y_pos = ly - winy;
    }

    u_int16 tile_id;

    for (int pixel = 0; pixel < 160; ++pixel) {
        x_pos = pixel + scrollx;

        if (using_window && pixel >= winx) {
            x_pos = pixel - winx;
        }

        tile_id = ((y_pos / 8 * 32) + (x_pos / 8));

        u_int16 tile_address = bg_map + tile_id;
        short tile_num;

        if (unsign) {
            tile_num = (u_int8)memory[tile_address];
        } else {
            tile_num = (signed char)memory[tile_address];
        }

        u_int16 tile_location = tile_data;

        if (unsign) {
            tile_location += tile_num * 16;
        } else {
            tile_location += (tile_num + 128) * 16;
        }

        u_int8 line = y_pos % 8;
        line = line * 2;
        u_int8 byte1 = memory[tile_location + line];
        u_int8 byte2 = memory[tile_location + line + 1];

        int bit = x_pos % 8;
        bit = bit - 7;
        bit = bit * -1;

        int colour_number1 = (byte1 >> bit) & 1;
        int colour_number2 = (byte2 >> bit) & 1;
        int colour_number = (colour_number2 << 1) | colour_number1;

        int red = 0;
        int green = 0;
        int blue = 0;

        u_int8 pallete = memory[0xff47];
        int colour_id;

        switch (colour_number) {
            case 0: colour_id = pallete & 0x03; break;
            case 1: colour_id = (pallete & 0x0C) >> 2; break;
            case 2: colour_id = (pallete & 0x30) >> 4; break;
            case 3: colour_id = (pallete & 0xC0) >> 6; break;
        }

        char shade;

        if (colour_id == 0) {
            shade = 'W';
        } else if (colour_id == 1) {
            shade = 'L';
        } else if (colour_id == 2) {
            shade = 'D';
        } else {
            shade = 'B';
        }

        switch (shade) {
            case 'W': red = 255; blue = 255; green = 255; break;
            case 'L': red = 0xCC; blue = 0xCC; green = 0xCC; break;
            case 'D': red = 0x77; blue = 0x77; green = 0x77; break;
            default: break;
        }

        if (ly < 0 || ly > 143) {
            continue;
        }

        screen[pixel][ly][0] = red;
        screen[pixel][ly][1] = green;
        screen[pixel][ly][2] = blue;
    }
}

void draw_sprites()
{

}

void draw_frame()
{
    int count = 0;
    for (int i = 0; i < 144; ++i) {
        for (int j = 0; j < 160; ++j) {
            pixels[count++] = screen[j][i][0];
            pixels[count++] = screen[j][i][1];
            pixels[count++] = screen[j][i][2];
        }
    }

    screen_surface = SDL_CreateRGBSurfaceFrom((void*)pixels,160,144,24,160*3,0x0000ff,0x00ff00,0xff0000,0x0);
    texture = SDL_CreateTextureFromSurface(renderer,screen_surface);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    //SDL_Delay(1000);
}
