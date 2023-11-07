/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
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

void handle_keydown(int n)
{
    if (is_set(joypad, n)) {
        request_interrupt(4);
    }
    reset_bit(&joypad, n);
}

#ifndef EMSCRIPTEN
void on_keydown(SDL_Keycode code)
{
    switch (code) {
        case SDLK_KP_PLUS: cap = 0; break;
        case SDLK_q: debug = !debug; break;
        case SDLK_p: paused = !paused; break;
        case SDLK_n: next = 1; break;
        case SDLK_LEFT: handle_keydown(1); break;
        case SDLK_RIGHT: handle_keydown(0); break;
        case SDLK_UP: handle_keydown(2); break;
        case SDLK_DOWN: handle_keydown(3); break;
        case SDLK_RETURN: handle_keydown(7); break;
        case SDLK_z: handle_keydown(5); break;
        case SDLK_x: handle_keydown(4); break;
        case SDLK_RSHIFT: handle_keydown(6); break;
        case SDLK_s: save_request = 1; break;
        case SDLK_l: load_request = 1; break;
        case SDLK_r: restart(); break;
    }
}

void on_keyup(SDL_Keycode code)
{
    switch (code) {
        case SDLK_KP_PLUS: cap = 1; break;
        case SDLK_LEFT: set_bit(&joypad,1); break;
        case SDLK_RIGHT: set_bit(&joypad,0); break;
        case SDLK_UP: set_bit(&joypad,2); break;
        case SDLK_DOWN: set_bit(&joypad,3); break;
        case SDLK_RETURN: set_bit(&joypad,7); break;
        case SDLK_z: set_bit(&joypad,5); break;
        case SDLK_x: set_bit(&joypad,4); break;
        case SDLK_RSHIFT: set_bit(&joypad,6); break;
    }
}

// updates contents of joypad register with user input
void update_joypad(SDL_KeyboardEvent *key)
{
    switch (key->type) {
        case SDL_KEYDOWN: on_keydown(key->keysym.sym); break; 
        case SDL_KEYUP: on_keyup(key->keysym.sym); break;
    }
}
#endif

#ifdef EMSCRIPTEN
extern "C" {
EMSCRIPTEN_KEEPALIVE
void keyPress(int keyCode)
{
    handle_keydown(keyCode);
}

EMSCRIPTEN_KEEPALIVE
void keyRelease(int keyCode)
{
    set_bit(&joypad, keyCode);
}

EMSCRIPTEN_KEEPALIVE
void setSaveRequest()
{
    save_request = 1;
}

EMSCRIPTEN_KEEPALIVE
void setLoadRequest()
{
    load_request = 1;
}

EMSCRIPTEN_KEEPALIVE
void restartPress()
{
    restart();
}
}
#endif