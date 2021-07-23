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

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include "memory.h"

u_int8 memory[MEM_SIZE] = {0}; // 16-bit memory
u_int8 cart_rom[4194304] = {0};
u_int8 cart_ram[32768] = {0};
u_int8 rtc_reg[5] = {0};
u_int8 bank = 1; // current memory bank
u_int8 ram_bank = 0; // current ram bank
u_int8 rtc_select = 0;
int ram_enabled = 0; // enable/disable ram
int bank_mode = 0; // banking mode
cart_headers headers;

// real-time clock registers
u_int8 rtc_s;
u_int8 rtc_m;
u_int8 rtc_h;
u_int8 rtc_dl;
u_int8 rtc_dh;

#ifdef EMSCRIPTEN
EM_JS(void, call_ram_changed, (int address, int byte), {
	ram_changed(address, byte);
});

void ram_changed(u_int16 address, u_int8 byte)
{
    call_ram_changed(address, byte);
}
#endif
#ifndef EMSCRIPTEN
void ram_changed(u_int16 address, u_int8 byte)
{
    
}
#endif

// returns real-time clock
u_int8 get_rtc()
{
    switch (ram_bank) {
        case 0x8: return rtc_s;
        case 0x9: return rtc_m;
        case 0xA: return rtc_h;
        case 0xB: return rtc_dl;
        case 0xC: return rtc_dh;
    }
}

// sets real-time clock
void write_rtc(u_int8 regi, u_int8 byte)
{
    switch (regi) {
        case 0x8: rtc_s = byte;
        case 0x9: rtc_m = byte;
        case 0xA: rtc_h = byte;
        case 0xB: rtc_dl = byte;
        case 0xC: rtc_dh = byte;
    }
}

void init_sound_regs()
{
    write_memory(0xFF10, 0x80);
    write_memory(0xFF11, 0xBF);
    write_memory(0xFF12, 0xF3);
    write_memory(0xFF14, 0xBF);
    write_memory(0xFF16, 0x3F);
    write_memory(0xFF17, 0x00);
    write_memory(0xFF19, 0xBF);
    write_memory(0xFF1A, 0x7F);
    write_memory(0xFF1B, 0xFF);
    write_memory(0xFF1C, 0x9F);
    write_memory(0xFF1E, 0xBF);
    write_memory(0xFF20, 0xFF);
    write_memory(0xFF21, 0x00);
    write_memory(0xFF22, 0x00);
    write_memory(0xFF23, 0xBF);
    write_memory(0xFF24, 0x77);
    write_memory(0xFF25, 0xF3);
    write_memory(0xFF26, 0xF1);
}

void init_wave_ram() {
    for (int i = 0xFF30; i <= 0xFF3F; ++i) {
        write_memory(i, i % 2 == 0 ? 0 : 0xFF);
    }
}

void clear_sound_regs() {
    for (int i = 0xFF10; i <= 0xFF25; ++i) {
        write_memory(i, 0);
    }
}