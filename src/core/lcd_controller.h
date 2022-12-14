#ifndef LCD_CONTROLLER
#define LCD_CONTROLLER

#include "system_types.h"

typedef struct {
    int index;
    int auto_inc;
    u8 palette[64];
} Palette;

class LCDController
{
    public:
    int fps_count = 0;
    Palette bg_palette, spr_palette;

    void draw_frame();
    void update(int);
    void switch_mode(int);
};

#endif //LCD_CONTROLLER
