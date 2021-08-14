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

#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

struct Pixel {
    int red;
    int green;
    int blue;
};

struct Tile {
    struct Pixel px;
    u_int8 attributes;
    int priority;
    int v_flip;
    int h_flip;
    int bank;
    int palette;
    int colour_number;
    u_int8 byte1;
    u_int8 byte2;
    int transparent;
};

struct Sprite {
    struct Pixel px;
    u_int8 y_pos;
    u_int8 x_pos;
    u_int8 tile_number;
    u_int8 attributes;
    int bank;
    int palette;
    int priority;
    int colour_number;
    u_int16 address;
    u_int8 byte1;
    u_int8 byte2;
    int transparent;
};

extern int interrupt_cycles[];

void update_graphics(void);
void draw_tiles(void);
void draw_sprites(void);
void draw_frame(void);
void switch_mode(int);
void compare_ly(void);
void inc_ly(void);
int get_mode(void);
void update_lcd(int);
void sort_sprites(struct Sprite[], int);
void draw_blank(void);

#endif // LCD_CONTROLLER_H
