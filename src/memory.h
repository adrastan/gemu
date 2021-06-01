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
#include "sound.h"

#define MEM_SIZE 0x10000

typedef struct {
    int mbc;
    u_int8 logo[48];
    u_int8 title[16];
    int cgb;
    int sgb;
    int rom_size;
    int ram_size;
} cart_headers;

extern u_int8 memory[];
extern u_int8 bank;
extern u_int8 ram_bank;
extern u_int8 cart_rom[];
extern u_int8 cart_ram[];
extern int ram_enabled;
extern cart_headers headers;
extern int bank_mode;
extern int start_transfer;
extern int fps_count;
extern int transfer;

static inline void do_banking(u_int16, u_int8);
static inline void do_dma(u_int8);
u_int8 get_rtc(void);
void write_rtc(u_int8,u_int8);
static inline void do_sound(u_int16,u_int8);
void init_sound_regs(void);
void clear_sound_regs(void);
void init_wave_ram(void);


static inline u_int8 mbc1_read(u_int16 address)
{
    // returns byte depending on rom bank
    if (address >= 0x4000 && address <= 0x7FFF) {
        return cart_rom[(address - 0x4000) + (bank * 0x4000)];
    }
    // returns ram byte depending on ram bank
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (!ram_enabled) {
            return 0xFF;
        }
        return cart_ram[(address - 0xA000) + (ram_bank * 8192)];
    } else {
        return memory[address];
    }
}

static inline void mbc1_write(u_int16 address, u_int8 byte)
{
    static int rom_bank_lower = 0;
    static int rom_bank_upper = 0;

    if (address >= 0 && address <= 0x1FFF) {
        if ((byte & 0x0A) == 0x0A) {
            ram_enabled = 1;
        } else {
            ram_enabled = 0;
        }
    } 
    else if (address >= 0x2000 && address <= 0x3FFF) {
        if (!byte && bank_mode == 0) {
            byte = 1;
        }
        if (headers.rom_size == 1) {
            rom_bank_lower = byte & 0x7;
        } else if (headers.rom_size == 2) {
            rom_bank_lower = byte & 0xF;
        } else if (headers.rom_size == 3) {
            rom_bank_lower = byte & 0xF;
        } else {
            rom_bank_lower = byte & 0x1F;
        }
        bank = rom_bank_lower;
    } 
    else if (address >= 0x4000 && address <= 0x5FFF) {
        if (bank_mode == 0 && headers.rom_size >= 5) {
            rom_bank_upper = byte & 3;
            bank = (rom_bank_upper << 5) | rom_bank_lower;
        } else if (bank_mode == 1) {
            ram_bank = byte & 3;
        }
    } 
    else if (address >= 0x6000 && address <= 0x7FFF) {
        bank_mode = byte & 1;
    } 
    else if (address >= 0xA000 && address <= 0xBFFF) {
        if (!ram_enabled) {
            return;
        }
        cart_ram[(address - 0xA000) + (ram_bank * 8192)] = byte;
    } else {
        memory[address] = byte;
    }
}

static inline u_int8 mbc2_read(u_int16 address)
{

}

static inline void mbc2_write(u_int16 address, u_int8 byte)
{

}

static inline u_int8 read_memory(u_int16 address)
{
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

    if (headers.mbc == 0) {
        return memory[address];
    } else if (headers.mbc >= 1 && headers.mbc <= 3) {
        return mbc1_read(address);
    } else if (headers.mbc == 5 || headers.mbc == 6) {
        return mbc2_read(address);
    }
}

static inline void write_memory(u_int16 address, u_int8 byte)
{
    if (address >= 0xFF10 && address <= 0xFF3F) {
        do_sound(address, byte);
        return;
    }
    // ram area
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (!ram_enabled) return;
        if (ram_bank <= 3) {
            cart_ram[(address - 0xA000) + (ram_bank * 8192)] = byte;
        } else {
            write_rtc(ram_bank, byte);
        }
        return;
    }
    // DMA transfer
    if (address == 0xff46) {
        do_dma(byte);
        memory[address] = byte;
        return;
    }
    // joypad
    if (address == 0xff00) {
        memory[address] &= 0xCF;
        memory[address] |= (byte & 0x30);
        return;
    }
    // enable lcd
    if (address == 0xff40) {
        if (is_set(byte,7) && !is_set(memory[address],7)) {
            switch_mode(2);
            fps_count = 0;
        }
        memory[address] = byte;
        return;
    }
    // reset timers
    if (address == 0xff04) {
        memory[address] = 0;
        memory[address+1] = 0;
        return;
    }
    // echo memory
    if (address >= 0xE000 && address < 0xFE00) {
        memory[address] = byte;
        memory[address-0x2000] = byte;
        return;
    }
    // echo memory
    if (address >= 0xC000 && address < 0xDE00) {
        memory[address] = byte;
        memory[address+0x2000] = byte;
        return;
    }
    // unusable memory
    if (address >= 0xFEA0 && address <= 0xFEFF) {
        return;
    }

    if (headers.mbc == 0 && address >= 0x8000) {
        memory[address] = byte;
    } else if (headers.mbc >= 1 && headers.mbc <= 3) {
        mbc1_write(address, byte);
    } else if (headers.mbc == 5 || headers.mbc == 6) {
        mbc2_write(address, byte);
    }
}

// transfer sprite data to oam memory
static inline void do_dma(u_int8 byte)
{
    int b = (int)byte << 8;
    int c = 0x00;
    for (int i = b; i < b + 0xA0; ++i) {
        write_memory(0xfe00+c, read_memory(i));
        ++c;
    }
    return;
}

static inline void do_sound(u_int16 address, u_int8 byte)
{
    sync_sound(address, byte);
}

#endif // MEMORY_H