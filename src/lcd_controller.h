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

struct sprite {
    u_int8 y_pos;
    u_int8 x_pos;
    u_int8 tile_number;
    u_int8 attributes;
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
void sort_sprites(struct sprite[], int);
void draw_blank(void);
int get_bg_colour(int);

#endif // LCD_CONTROLLER_H
