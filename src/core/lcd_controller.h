#ifndef LCD_CONTROLLER
#define LCD_CONTROLLER

#define MODE_0 204
#define MODE_2 80
#define MODE_3 172
#define TOTAL 456

#include "system_types.h"
#include "memory.h"

typedef struct {
    int index;
    int auto_inc;
    u8 palette[64];
} Palette;


class Memory;

class LCDController
{
    public:
    LCDController(Memory * const);
    int fps_count = 0;
    int interrupt_cycles[4] = {MODE_0, MODE_2, MODE_3, TOTAL};
    Palette bg_palette, spr_palette;
    Memory *memory;

    void draw_frame();
    void update(int);
    void switch_mode(int);
    int get_mode();
};

#endif //LCD_CONTROLLER
