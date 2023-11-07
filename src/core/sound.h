/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#ifndef SOUND_H
#define SOUND_H

#include "cpu_types.h"

static void handle_error( const char*);
void init_sound(void);
void update_sound(int);
void sync_sound(u_int16, u_int8);
void update_apu(u_int16, u_int8);

#endif // SOUND_H
