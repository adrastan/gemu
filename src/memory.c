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

#include "memory.h"
#include "sound.h"

u_int8 memory[MEM_SIZE] = {0}; // 16-bit memory
u_int8 cart_rom[4194304] = {0};
u_int8 cart_ram[32768] = {0};
u_int8 bank = 1; // current memory bank
u_int8 ram_bank = 0; // current ram bank
int ram_enabled = 0; // enable/disable ram
int bank_mode = 0; // banking mode
int MBC2; // memory bank controller 2
int MBC3; // memory bank controller 3

// real-time clock registers
u_int8 rtc_s;
u_int8 rtc_m;
u_int8 rtc_h;
u_int8 rtc_dl;
u_int8 rtc_dh;

void do_ram_enable(u_int16 address, u_int8 byte)
{
    // lsb of upper address byte must not be zero to enable/disable RAM
    if (MBC2 && (address & (1 << 8))) {
        return;
    }
    if (MBC3) {
        if (byte == 0x0A) {
            ram_enabled = 1;
        }
        if (byte == 0) {
            ram_enabled = 0;
        }
    } else {
        byte &= 0x0F;
        if (byte == 0x0A) {
            ram_enabled = 1;
        } else {
            ram_enabled = 0;
        }
    }
}

void do_rom_bank_lower(u_int16 address, u_int8 byte)
{
    if (MBC2 && (address & (1 << 8))) {
        u_int8 lower_4 = byte & 0x0F;
        bank = 0;
        bank |= lower_4;
        if (bank == 0) {
            ++bank;
        }
    } 
    else if (MBC3) {
        bank = byte & 0x7F;
        if (bank == 0) {
            ++bank;
        }
    } 
    else {
        u_int8 lower_5 = byte & 0x1F;
        bank &= 0xE0;
        bank |= lower_5;
        if (bank == 0 || bank == 0x20 || bank == 0x40 || bank == 0x60) {
            ++bank;
        }
    }
}

void do_rom_bank_upper(u_int16 address, u_int8 byte)
{
    if (MBC2) {
        return;
    }
    else if (MBC3) {
        if ((byte >= 0 && byte <= 3) || (byte >= 0x8 && byte <= 0xC)) {
            ram_bank = byte;
        }
        return;
    }
    if (bank_mode == 0) {
        u_int8 upper_2 = byte & 0x60;
        bank &= 0x9F;
        bank |= (upper_2);
        if (bank == 0 || bank == 0x20 || bank == 0x40 || bank == 0x60) {
            ++bank;
        }
    } else {
        if (byte > 3) {
            return;
        }
        ram_bank = byte;
    }
}

void do_mode_select(u_int16 address, u_int8 byte)
{
    if (MBC2) {
        return;
    }
    byte &= 1;
    bank_mode = byte;
    if (bank_mode == 1) {
        bank &= 0x9F;
    }
    if (bank_mode == 0) {
        ram_bank = 0;
    }
}

void do_banking(u_int16 address, u_int8 byte)
{
    // RAM enable
    if (address >= 0 && address <= 0x1FFF) {
        do_ram_enable(address, byte);
    }
    // select rom bank lower
    else if (address >= 0x2000 && address <= 0x3FFF) {
        do_rom_bank_lower(address, byte);
    }
    // select rom bank upper
    else if (address >= 0x4000 && address <= 0x5FFF) {
        do_rom_bank_upper(address, byte);
    }
    // mode select
    else if (address >= 0x6000 && address <= 0x7FFF) {
        do_mode_select(address, byte);
    }
}

// transfer sprite data to oam memory
void do_dma(u_int8 byte)
{
    int b = (int)byte << 8;
    int c = 0x00;
    for (int i = b; i < b + 0xA0; ++i) {
        write_memory(0xfe00+c, read_memory(i));
        ++c;
    }
    return;
}

void do_sound(u_int16 address, u_int8 byte)
{
    sync_sound(address, byte);
}

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
    memory[0xFF10] = 0x80;
    memory[0xFF11] = 0xBF;
    memory[0xFF12] = 0xF3;
    memory[0xFF14] = 0xBF;
    memory[0xFF16] = 0x3F;
    memory[0xFF17] = 0x00;
    memory[0xFF19] = 0xBF;
    memory[0xFF1A] = 0x7F;
    memory[0xFF1B] = 0xFF;
    memory[0xFF1C] = 0x9F;
    memory[0xFF1E] = 0xBF;
    memory[0xFF20] = 0xFF;
    memory[0xFF21] = 0x00;
    memory[0xFF22] = 0x00;
    memory[0xFF23] = 0xBF;
    memory[0xFF24] = 0x77;
    memory[0xFF25] = 0xF3;
    memory[0xFF26] = 0xF1;
}

