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

#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include "joypad.h"
#include "registers.h"
#include "lcd_controller.h"
#include "cpu.h"

#define MEM_SIZE 0x10000

extern u_int8 memory[];
extern u_int8 bank;
extern u_int8 ram_bank;
extern u_int8 cart_rom[];
extern u_int8 cart_ram[];
extern int ram_enabled;
extern int MBC2;
extern int MBC3;
extern int bank_mode;
extern int start_transfer;
extern int fps_count;
extern int transfer;

void do_banking(u_int16, u_int8);
void do_dma(u_int8);
u_int8 get_rtc(void);
void write_rtc(u_int8,u_int8);
void do_sound(u_int16,u_int8);
void init_sound_regs(void);
void clear_sound_regs(void);
void init_wave_ram(void);

static inline u_int8 _read_ram(u_int16 address)
{
    if (!ram_enabled) {
        return 0xFF;
    }
    // using mbc2
    if (MBC2) {
        return cart_ram[(address - 0xA000)] & 0x0F;
    }
    // using mbc3
    if (MBC3 && ram_bank <= 3) {
        return cart_ram[(address - 0xA000) + (ram_bank * 8192)];
    } else if (MBC3) {
        return get_rtc();
    }
    return cart_ram[(address - 0xA000) + (ram_bank * 8192)];
}

static inline u_int8 read_memory(u_int16 address)
{
    // returns byte depending on rom bank
    if (address >= 0x4000 && address <= 0x7FFF) {
        return cart_rom[(address - 0x4000) + (bank * 0x4000)];
    }
    // returns ram byte depending on ram bank
    if (address >= 0xA000 && address <= 0xBFFF) {
        return _read_ram(address);
    }
    if (address == 0xff00) {
        return joypad_state();
    }

    if (address == 0xff10) {
        return memory[address] | 0x80;
    }
    if (address == 0xff11) {
        return memory[address] | 0x3F;
    }
    if (address == 0xff12) {
        return memory[address] | 0x00;
    }
    if (address == 0xff13) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff14) {
        return memory[address] | 0xBF;
    }

    if (address == 0xff15) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff16) {
        return memory[address] | 0x3F;
    }
    if (address == 0xff17) {
        return memory[address] | 0x00;
    }
    if (address == 0xff18) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff19) {
        return memory[address] | 0xBF;
    }

    if (address == 0xff1a) {
        return memory[address] | 0x7F;
    }
    if (address == 0xff1b) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff1c) {
        return memory[address] | 0x9F;
    }
    if (address == 0xff1d) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff1e) {
        return memory[address] | 0xBF;
    }

    if (address == 0xff1f) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff20) {
        return memory[address] | 0xFF;
    }
    if (address == 0xff21) {
        return memory[address] | 0x00;
    }
    if (address == 0xff22) {
        return memory[address] | 0x00;
    }
    if (address == 0xff23) {
        return memory[address] | 0xBF;
    }

    if (address == 0xff24) {
        return memory[address] | 0x00;
    }
    if (address == 0xff24) {
        return memory[address] | 0x00;
    }
    if (address == 0xff24) {
        return memory[address] | 0x70;
    }

    if (address >= 0xff27 && address <= 0xff2f) {
        return 0xff;
    }
    return memory[address];
}

static inline void write_memory(u_int16 address, u_int8 byte)
{
    if (address >= 0xFF10 && address <= 0xFF3F) {
        do_sound(address, byte);
    }
    // ram area
    else if (address >= 0xA000 && address <= 0xBFFF) {
        if (!ram_enabled) return;
        if (ram_bank <= 3) {
            cart_ram[(address - 0xA000) + (ram_bank * 8192)] = byte;
        } else {
            write_rtc(ram_bank, byte);
        }
    }
    // DMA transfer
    else if (address == 0xff46) {
        do_dma(byte);
        memory[address] = byte;
    }
    // joypad
    else if (address == 0xff00) {
        memory[address] &= 0xCF;
        memory[address] |= (byte & 0x30);
    }
    // enable lcd
    else if (address == 0xff40) {
        if (is_set(byte,7) && !is_set(memory[address],7)) {
            switch_mode(2);
            fps_count = 0;
        }
        memory[address] = byte;
    }
    // reset timers
    else if (address == 0xff04) {
        memory[address] = 0;
        memory[address+1] = 0;
    }
    // banking control
    else if (address < 0x8000) {
        do_banking(address, byte);
    }
    // echo memory
    else if (address >= 0xE000 && address < 0xFE00) {
        memory[address] = byte;
        memory[address-0x2000] = byte;
    }
    // echo memory
    else if (address >= 0xC000 && address < 0xDE00) {
        memory[address] = byte;
        memory[address+0x2000] = byte;
    }
    // unusable memory
    else if (address >= 0xFEA0 && address <= 0xFEFF) {
        return;
    }
    else {
        memory[address] = byte;
    }
}

#endif // MEMORY_H