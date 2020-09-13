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

#include "joypad.h"
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"

u_int8 joypad = 0xFF;

// returns the current state of the joypad register
u_int8 joypad_state()
{
    if (!is_set(memory[0xff00],4)) {
        u_int8 dir_buttons = joypad & 0xF;
        dir_buttons |= 0xF0;
        return dir_buttons & memory[0xff00];
    } else if (!is_set(memory[0xff00],5)) {
        u_int8 std_buttons = joypad & 0xF0;
        std_buttons >>= 4;
        std_buttons |= 0xF0;
        return std_buttons & memory[0xff00];
    }
    return 0xff;
}

// updates contents of joypad register with user input
void update_joypad(SDL_KeyboardEvent *key)
{
    switch (key->type) {
        case SDL_KEYDOWN:
            switch (key->keysym.sym) {
                case SDLK_KP_PLUS: cap = 0; break;
                case SDLK_q: debug = 1; break;
                case SDLK_LEFT: if (is_set(joypad,1)) { request_interrupt(4); }
                    reset_bit(&joypad,1); break;
                case SDLK_RIGHT: if (is_set(joypad,0)) { request_interrupt(4); }
                    reset_bit(&joypad,0); break;
                case SDLK_UP: if (is_set(joypad,2)) { request_interrupt(4); }
                    reset_bit(&joypad,2); break;
                case SDLK_DOWN: if (is_set(joypad,3)) { request_interrupt(4); }
                    reset_bit(&joypad,3); break;
                case SDLK_RETURN: if (is_set(joypad,7)) { request_interrupt(4); }
                    reset_bit(&joypad,7); break;
                case SDLK_z: if (is_set(joypad,5)) { request_interrupt(4); }
                    reset_bit(&joypad,5); break;
                case SDLK_x: if (is_set(joypad,4)) { request_interrupt(4); }
                    reset_bit(&joypad,4); break;
                case SDLK_RSHIFT: if (is_set(joypad,6)) { request_interrupt(4); }
                    reset_bit(&joypad,6); break;
                case SDLK_s: save_request = 1; break;
                case SDLK_l: load_request = 1; break;
                case SDLK_r: restart(); break;
            } break;
        case SDL_KEYUP:
            switch (key->keysym.sym) {
                case SDLK_KP_PLUS: cap = 1; break;
                case SDLK_LEFT: set_bit(&joypad,1); break;
                case SDLK_RIGHT: set_bit(&joypad,0); break;
                case SDLK_UP: set_bit(&joypad,2); break;
                case SDLK_DOWN: set_bit(&joypad,3); break;
                case SDLK_RETURN: set_bit(&joypad,7); break;
                case SDLK_z: set_bit(&joypad,5); break;
                case SDLK_x: set_bit(&joypad,4); break;
                case SDLK_RSHIFT: set_bit(&joypad,6); break;
            } break;
    }
}
