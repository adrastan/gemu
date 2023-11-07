/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
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
