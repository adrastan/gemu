/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#ifndef EMSCRIPTEN
#include <SDL2\SDL.h>
extern SDL_Window* window;
extern SDL_Surface* screen_surface;
extern SDL_Texture* texture;
extern SDL_Renderer* renderer;
#endif

#ifdef EMSCRIPTEN
extern unsigned char pixels[160 * 144 * 3];
#endif

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"
#include "lcd_controller.h"
#include "interrupts.h"

#define MODE_0 204
#define MODE_2 80
#define MODE_3 172
#define TOTAL 456

u_int8 screen[144][160][3] = {};
// u_int8 pixels[160*144*3] = {};
int pixel_count = 0;
int interrupt_cycles[4] = {MODE_0, MODE_2, MODE_3, TOTAL};

void draw_pixel(int, int, int, int);
u_int16 get_sprite_address(int, struct Sprite *);
void render_sprites(int, u_int8, int, int, struct Sprite []);
void update_pixel(int);
void get_pixel(int);
struct Tile get_tile(int);
struct Sprite get_sprite(int);
int compare_pixel(struct Tile *, struct Sprite *);
struct Pixel get_colour(int, int, Palette *, u_int8);

void update_graphics()
{
    int ly = memory[0xff44];
    if (ly < 0 || ly > 143) {
        return;
    }

    for (int pixel = 0; pixel < 160; ++pixel) {
        update_pixel(pixel);
    }
}

void update_pixel(int pixel)
{
    int ly = memory[0xff44];

    if (!headers.cgb && !(memory[0xff40] & 1)) {
        draw_pixel(pixel, 0xFF, 0xFF, 0xFF);
    } else {
        get_pixel(pixel);
    }
}

void get_pixel(int pixel)
{
    int ly = memory[0xff44];

    struct Tile tile = get_tile(pixel);
    struct Sprite spr = get_sprite(pixel);
    draw_pixel(pixel, tile.px.red, tile.px.green, tile.px.blue);

    // if sprites are enabled draw sprites
    if ((memory[0xff40] & 0x2) && !spr.transparent) {
        if (headers.cgb && !tile.priority) {
            if (!spr.priority || (spr.priority && tile.colour_number == 0)) {
                draw_pixel(pixel, spr.px.red, spr.px.green, spr.px.blue);
            }
        } else if (!spr.priority || tile.colour_number == 0) {
            draw_pixel(pixel, spr.px.red, spr.px.green, spr.px.blue);
        }
    }
}

struct Tile get_tile(int pixel)
{
    struct Tile tile;
    int ly = memory[0xff44];

    u_int8 scrolly = memory[0xff42];
    u_int8 scrollx = memory[0xff43];
    int winy = memory[0xff4a];
    int winx = memory[0xff4b] - 7;
    int using_window = 0;
    int unsign = 1;
    u_int16 bg_map;
    u_int16 win_map;
    u_int16 tile_data;
    u_int8 lcdc = memory[0xff40];


    // check if window is enabled and the current scanline
    // falls on the window
    if (is_set(lcdc,5) && winy <= ly) {
         using_window = 1;
    }

    // set window tile map
    if (using_window) {
        win_map = is_set(lcdc,6) ? 0x9C00 : 0x9800;
    }

    // set background tile map
    bg_map = is_set(lcdc,3) ? 0x9C00 : 0x9800;

    // set tile data address
    if (is_set(lcdc,4)) {
        tile_data = 0x8000;
    } else {
        tile_data = 0x8800;
        unsign = 0;
    }

    u_int8 y_pos, x_pos;
    u_int16 tile_id, tile_address, tile_location;
    int on_window;
    u_int8 attributes;

    // check if current pixel falls on the window
    if (using_window && pixel >= winx) {
        x_pos = pixel - winx;
        y_pos = ly - winy;
        tile_id = ((y_pos / 8 * 32) + (x_pos / 8));
        tile_address = win_map + tile_id;
        on_window = 1;
    } else {
        x_pos = pixel + scrollx;
        y_pos = scrolly + ly;
        tile_id = ((y_pos / 8 * 32) + (x_pos / 8));
        tile_address = bg_map + tile_id;
        on_window = 0;
    }

    if (headers.cgb) {
        attributes = vram[(tile_address - 0x8000) + 0x2000];
        tile.priority = is_set(attributes, 7);
        tile.v_flip = is_set(attributes, 6);
        tile.h_flip = is_set(attributes, 5);
        tile.bank = is_set(attributes, 3);
        tile.palette = attributes & 7;
        tile.attributes = attributes;
    }

    short tile_num;

    // using tile numbers 0-255
    if (unsign) {
        tile_num = (u_int8)vram[tile_address - 0x8000];
    // using tile numbers -128-127
    } else {
        tile_num = (signed char)vram[tile_address - 0x8000];
    }

    tile_location = tile_data;

    if (unsign) {
        tile_location += tile_num * 16;
    } else {
        tile_location += (tile_num + 128) * 16;
    }

    // get current line and their bytes from memory
    u_int8 line = y_pos % 8;
    if (headers.cgb && tile.v_flip) {
        line = line - 7;
        line *= -1;
    }
    u_int8 byte1, byte2;
    line *= 2;
    if (headers.cgb) {
        byte1 = vram[((tile_location + line) - 0x8000) + tile.bank * 0x2000];
        byte2 = vram[((tile_location + line + 1) - 0x8000) + tile.bank * 0x2000];
    } else {
        byte1 = vram[tile_location + line - 0x8000];
        byte2 = vram[tile_location + line + 1 - 0x8000];
    }

    // bit 7 is leftmost pixel
    int bit = x_pos % 8;
    bit -= 7;
    bit *= -1;
    if (headers.cgb && tile.h_flip) {
        bit -= 7;
        bit *= -1;
    }


    // colour number is bit of first byte and second byte
    int colour_number1 = (byte1 >> bit) & 1;
    int colour_number2 = (byte2 >> bit) & 1;
    int colour_number = (colour_number2 << 1) | colour_number1;
    tile.colour_number = colour_number;
    tile.byte1 = byte1;
    tile.byte2 = byte2;
    tile.px = get_colour(colour_number, tile.palette, &bg_palette, memory[0xff47]);

    return tile;
}

struct Sprite get_sprite(int pixel)
{
    struct Sprite sprite;
    u_int8 ly = memory[0xff44];
    u_int8 lcdc = memory[0xff40];
    u_int8 sprites_on_scanline[10][4] = {{0}};
    int sprite_size = 8;
    int count = 0;
    u_int8 sprites[40][4];
    int pallete_num;
    u_int8 pallete;
    int colour_number1, colour_number2;

    // get all sprites from oam memory
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 4; ++j) {
            sprites[i][j] = memory[0xFE00+count];
            ++count;
        }
    }
    int sprite_count = 0;

    if (is_set(lcdc,2)) {
        sprite_size = 16;
    }

    // filter sprites that fall on the current scanline
    int offset = sprite_size == 8 ? 8 : 0;
    for (int i = 0; i < 40; ++i) {
        if (ly < sprites[i][0] - 16 || ly >= (sprites[i][0] - offset)) {
            continue;
        }
        sprites_on_scanline[sprite_count][0] = sprites[i][0];
        sprites_on_scanline[sprite_count][1] = sprites[i][1];
        sprites_on_scanline[sprite_count][2] = sprites[i][2];
        sprites_on_scanline[sprite_count][3] = sprites[i][3];
        ++sprite_count;
        if (sprite_count == 10) {
            break;
        }
    }
    struct Sprite sprites_on_pixel[sprite_count];

    count = 0;
    // get all sprites which fall on the current pixel
    for (int i = 0; i < sprite_count; ++i) {
        if (pixel >= sprites_on_scanline[i][1] - 8 && pixel < sprites_on_scanline[i][1]) {
            sprites_on_pixel[count].tile_number = sprites_on_scanline[i][2];
            sprites_on_pixel[count].x_pos = sprites_on_scanline[i][1];
            sprites_on_pixel[count].y_pos = sprites_on_scanline[i][0];
            sprites_on_pixel[count].attributes = sprites_on_scanline[i][3];
            ++count;
        }
    }

    if (count == 0) {
        sprite.transparent = 1;
        return sprite;
    }

    for (int i = 0; i < count; ++i) {
        sprite = sprites_on_pixel[i];

        if (!headers.cgb) {
            sort_sprites(sprites_on_pixel, count);
        }

        u_int8 attributes = sprite.attributes;
        u_int8 x_pos = sprite.x_pos;
        u_int8 y_pos = sprite.y_pos;

        sprite.priority = is_set(attributes, 7);
        sprite.bank = is_set(attributes, 3);
        sprite.address = get_sprite_address(sprite_size, &sprite);

        if (!headers.cgb) {
            sprite.byte1 = vram[sprite.address - 0x8000];
            sprite.byte2 = vram[sprite.address - 0x8000 + 1];
        } else {
            sprite.byte1 = vram[(sprite.address - 0x8000) + sprite.bank * 8192];
            sprite.byte2 = vram[(sprite.address - 0x8000 + 1) + sprite.bank * 8192];
        }
        
        int bit;
        // check if sprite is flipped horizontally
        if (is_set(attributes, 5)) {
            bit = (x_pos - pixel - 8) * -1;
        } else {
            bit = (x_pos - pixel - 1);
        }

        if (headers.cgb) {
            pallete_num = attributes & 7;
        } else {
            pallete_num = is_set(attributes,4);
        }

        if (pallete_num) {
            pallete = memory[0xff49];
        } else {
            pallete = memory[0xff48];
        }

        colour_number1 = (sprite.byte1 >> bit) & 1;
        colour_number2 = (sprite.byte2 >> bit) & 1;
        sprite.colour_number = (colour_number2 << 1) | colour_number1;
        sprite.palette = pallete_num;
        sprite.px = get_colour(sprite.colour_number, sprite.palette, &spr_palette, pallete);

        if (sprite.colour_number != 0) {
            sprite.transparent = 0;
            return sprite;
        }
    }

    sprite.transparent = 1;
    return sprite;
}

int compare_pixel(struct Tile *tile, struct Sprite *sprite)
{
    return 0;
}

struct Pixel get_colour(int colour_number, int palette_index, Palette *palette, u_int8 gb_palette)
{
    struct Pixel px;
    int colour_id;

    // get correct colour from pallete
    switch (colour_number) {
        case 0: colour_id = gb_palette & 0x03; break;
        case 1: colour_id = (gb_palette & 0x0C) >> 2; break;
        case 2: colour_id = (gb_palette & 0x30) >> 4; break;
        case 3: colour_id = (gb_palette & 0xC0) >> 6; break;
    }

    int red = 0;
    int green = 0;
    int blue = 0;

    // shade for current pixel
    if (colour_id == 0) {
        red = green = blue = 0xFF;
    } else if (colour_id == 1) {
        red = green = blue = 0xCC;
    } else if (colour_id == 2) {
        red = green = blue = 0x77;
    }

    if (headers.cgb) {
        int index = palette_index * 8;
        u_int8 b1 = palette->palette[index + colour_number * 2];
        u_int8 b2 = palette->palette[index + (colour_number * 2) + 1];
        blue = b1 & 0x1f;
        green = (b1 >> 5) | ((b2 & 3) << 3);
        red = (b2 >> 2) & 0x1f;
        red *= 10;
        green *= 10;
        blue *= 10;
        if (red > 255) red = 255;
        if (green > 255) green = 255;
        if (blue > 255) blue = 255;
    }

    #ifdef EMSCRIPTEN
    int temp;
    temp = red;
    red = blue;
    blue = temp;
    #endif

    px.red = red;
    px.green = green;
    px.blue = blue;

    return px;
}

#ifndef EMSCRIPTEN
void draw_pixel(int pixel, int red, int green, int blue)
{
    u_int8 ly = memory[0xff44];
    // set pixel value
    screen[ly][pixel][0] = red;
    screen[ly][pixel][1] = green;
    screen[ly][pixel][2] = blue;

    int idx = 160 * 3 * ly + 3 * pixel;
    unsigned char *p = (unsigned char *)screen_surface->pixels;

    p[idx + 0] = red;
    p[idx + 1] = green;
    p[idx + 2] = blue;
}
#endif

#ifdef EMSCRIPTEN
void draw_pixel(int pixel, int red, int green, int blue)
{
    u_int8 ly = memory[0xff44];
    // set pixel value
    screen[ly][pixel][0] = red;
    screen[ly][pixel][1] = green;
    screen[ly][pixel][2] = blue;

    int idx = 160 * 3 * ly + 3 * pixel;

    pixels[idx + 0] = red;
    pixels[idx + 1] = green;
    pixels[idx + 2] = blue;
}
#endif

u_int16 get_sprite_address(int sprite_size, struct Sprite *spr)
{
    int line;
    u_int8 tile_number, x_pos, y_pos, attributes;
    u_int16 tile_address_top, tile_address_bottom, tile_address;
    u_int8 ly = memory[0xff44];
    y_pos = spr->y_pos;
    x_pos = spr->x_pos;
    tile_number = spr->tile_number;
    attributes = spr->attributes;

    // if sprite size is 8x8
    if (sprite_size == 8) {
        tile_address = 0x8000 + tile_number * 16;
        if (is_set(attributes,6)) {
            line = (y_pos - 8) - ly - 1;
        } else {
            line = (y_pos - ly - 16) * -1;
        }
        line *= 2;
        return tile_address + line;
    }

    // sprite size is 8x16
    tile_address_top = 0x8000 + (tile_number & 0xFE) * 16;
    tile_address_bottom = 0x8000 + (tile_number | 0x01) * 16;

    // if scanline falls on the bottom tile
    if (ly >= (y_pos - 8)) {
        // check if the sprite is flipped vertically
        if (is_set(attributes,6)) {
            line = (y_pos - ly - 1) * 2;
            return tile_address_top + line;
        } else {
            line = (y_pos - ly - 8) * -2;
            return tile_address_bottom + line;
        }
    // scanline falls on the top tile
    } else {
        // check if sprite is flipped vertically
        if (is_set(attributes,6)) {
            line = (y_pos - 8 - ly - 1) * 2;
            return tile_address_bottom + line;
        } else {
            line = (y_pos - 16 - memory[0xff44]) * -2;
            return tile_address_top + line;
        }
    }
}


// sort sprites according to x values.
// sprites closer to the left will have higher priority
// and appear above the others.
void sort_sprites(struct Sprite spr[], int n)
{
    struct Sprite *temp;
    int j, k;
    for (j = 0; j < n - 1; ++j) {
        for (k = j + 1; k < n; ++k) {
            if (spr[k].x_pos < spr[j].x_pos) {
                temp = &spr[j];
                spr[j] = spr[k];
                spr[k] = *temp;
            }
        }
    }
}

#ifndef EMSCRIPTEN
// renders the next frame
void draw_frame()
{
    // screen_surface = SDL_CreateRGBSurfaceFrom((void*)pixels,160,144,24,160*3,0,0,0,0);
    // texture = SDL_CreateTextureFromSurface(renderer,screen_surface);
    // SDL_RenderClear(renderer);
    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    // SDL_RenderPresent(renderer);
    // SDL_FreeSurface(screen_surface);
    // SDL_DestroyTexture(texture);

    if (SDL_MUSTLOCK(screen_surface)) SDL_UnlockSurface(screen_surface);
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(screen_surface->pixels, 160, 144, 24, 160*3, 0, 0, 0, 0);
    SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(screenTexture);
}
#endif

// switch current lcd mode
void switch_mode(int mode)
{
    memory[0xff41] &= 0xfc;
    switch (mode) {
        case 0: break;
        case 1: memory[0xff41] |= 1; break;
        case 2: memory[0xff41] |= 2; break;
        case 3: memory[0xff41] |= 3; break;
    }
}

// compare lyc and ly registers
void compare_ly()
{
    if (memory[0xff45] == memory[0xff44]) {
        memory[0xff41] |= (1 << 2);
        if (is_set(memory[0xff41],6)) {
            request_interrupt(1);
        }
    } else {
        memory[0xff41] &= ~(1 << 2);
    }
}

// increments ly register
void inc_ly()
{
    ++memory[0xff44];
    if ((memory[0xff44]) == 154) {
        memory[0xff44] = 0;
    }
}

int get_mode()
{
    return memory[0xff41] & 0x03;
}

void update_lcd(int cycles)
{
    // reset lcd values if screen is disabled
    if (!(is_set(memory[0xff40],7))) {
        interrupt_cycles[3] = TOTAL;
        interrupt_cycles[0] = MODE_0;
        interrupt_cycles[1] = MODE_2;
        interrupt_cycles[2] = MODE_3;
        memory[0xff44] = 0;
        memory[0xff41] &= 0xf8;
        return;
    }

    int mode = get_mode();

    // currently in mode 2
    if (mode == 2) {
        interrupt_cycles[1] -= cycles;
        if (interrupt_cycles[1] <= 0) {
            interrupt_cycles[2] += interrupt_cycles[1];
            interrupt_cycles[1] = MODE_2;
            switch_mode(3);
        }
    // currently in mode 3
    } else if (mode == 3) {
        interrupt_cycles[2] -= cycles;
        if (interrupt_cycles[2] <= 0) {
            interrupt_cycles[0] += interrupt_cycles[2];
            interrupt_cycles[2] = MODE_3;
            switch_mode(0);
            if (memory[0xff44] < 144) {
                update_graphics();
                if (dma.active) {
                    do_hdma();
                }
            }
            if (is_set(memory[0xff41],3)) {
                request_interrupt(1);
            }
        }
    // currently in mode 0
    } else if (mode == 0) {
        interrupt_cycles[0] -= cycles;
        if (interrupt_cycles[0] <= 0) {
            if (memory[0xff44] < 144) {
                interrupt_cycles[1] += interrupt_cycles[0];
                interrupt_cycles[0] = MODE_0;
                switch_mode(2);
                if (is_set(memory[0xff41],5)) {
                    request_interrupt(1);
                }
            }
        }
    // currently in vblank ie mode 1
    } else {
        if (memory[0xff44] == 0) {
            switch_mode(2);
            if (is_set(memory[0xff41],5)) {
                request_interrupt(1);
            }
        }
    }

    interrupt_cycles[3] -= cycles;

    // increments ly every cycle through all modes
    if (interrupt_cycles[3] <= 0) {
        interrupt_cycles[3] += TOTAL;
        compare_ly();
        inc_ly();
    }
    // last scanline so switch to mode 1 and generate
    // vblank interrupt
    if (memory[0xff44] == 144 && mode != 1) {
        interrupt_cycles[0] = MODE_0;
        switch_mode(1);
        if (save_request) {
            save_state();
        }
        if (load_request) {
            load_state();
        }
        request_interrupt(0);
        if (is_set(memory[0xff41],4)) {
            request_interrupt(1);
        }
    }
}
