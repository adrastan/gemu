#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

struct sprite {
    u_int8 y_pos;
    u_int8 x_pos;
    u_int8 tile_number;
    u_int8 attributes;
};

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

#endif // LCD_CONTROLLER_H
