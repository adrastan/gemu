#ifndef JOYPAD_H
#define JOYPAD_H

#include <SDL2/SDL.h>
#include "cpu_types.h"

void update_joypad(SDL_KeyboardEvent*);
u_int8 joypad_state(void);

#endif // JOYPAD_H
