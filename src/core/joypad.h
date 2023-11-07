/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#ifndef JOYPAD_H
#define JOYPAD_H

#include <SDL.h>
void update_joypad(SDL_KeyboardEvent*);

#include "cpu_types.h"

u_int8 joypad_state(void);

#endif // JOYPAD_H
