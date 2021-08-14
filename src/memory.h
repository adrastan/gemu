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
    int index;
    int auto_inc;
    u_int8 palette[64];
} Palette;

typedef struct {
    int mbc;
    u_int8 logo[48];
    u_int8 title[16];
    int cgb;
    int sgb;
    int rom_size;
    int ram_size;
} cart_headers;

typedef struct {
    u_int16 source;
    u_int16 dest;
    int length;
    int active;
    int position;
} DMA;

extern u_int8 memory[];
extern int bank;
extern u_int8 ram_bank;
extern u_int8 rtc_select;
extern u_int8 cart_rom[];
extern u_int8 cart_ram[];
extern u_int8 rtc_reg[];
extern u_int8 vram[];
extern u_int8 wram[];
extern int wram_bank;
extern int vram_bank;
extern int ram_enabled;
extern cart_headers headers;
extern int bank_mode;
extern int start_transfer;
extern int fps_count;
extern int transfer;
extern int prepare_speed;
extern int double_speed;
extern int debug;

extern Palette bg_palette;
extern Palette spr_palette;
extern DMA dma;

static inline void do_oam_transfer(u_int8);
static inline void do_dma(u_int8);
static inline void do_gdma(u_int8);
static inline void do_hdma(void);
u_int8 get_rtc(void);
void write_rtc(u_int8,u_int8);
static inline void do_sound(u_int16,u_int8);
void init_sound_regs(void);
void clear_sound_regs(void);
void init_wave_ram(void);
void ram_changed(u_int16, u_int8);

static inline u_int8 read_memory(u_int16 address)
{
    if (debug && paused) {
        printf("READ: %x, BANK: %d, RAM BANK: %d, RAM_ENABLED: %d, VALUE: %x\n", address, bank, ram_bank, ram_enabled, memory[address]);
        fflush(stdout);
    }

    if (address >= 0 && address <= 0x3FFF) {
        return memory[address];
    }

    if (address >= 0x4000 && address <= 0x7FFF) {
        return cart_rom[(address - 0x4000) + (bank * 0x4000)];
    }

    // VRAM
    if (address >= 0x8000 && address <= 0x9FFF) {
        return vram[(address - 0x8000) + vram_bank * 0x2000];
    }

    // External ram
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (!ram_enabled) {
            return 0xFF;
        }
        if (headers.mbc == 2) {
            address &= 0x01FF;
        }
        return cart_ram[(address - 0xA000) + (ram_bank * 0x2000)];
    }

    // WRAM bank 0
    if (address >= 0xC000 && address <= 0xCFFF) {
        return wram[address - 0xC000];
    }

    // WRAM bank 1-7
    if (address >= 0xD000 && address <= 0xDFFF) {  
        return wram[(address - 0xD000) + wram_bank * 0x1000];
    }

    if (address == 0xff00) {
        return joypad_state();
    }

    if (address == 0xff55) {
        if (dma.active) {
            return (u_int8)((dma.length / 0x10) - 1);
        }
        return memory[0xff55];
    }

    return memory[address];
}

static inline void mbc1(u_int16 address, u_int8 byte)
{
    static u_int16 rom_bank_lower = 0;
    static u_int16 rom_bank_upper = 0;

    if (address >= 0 && address <= 0x1FFF) {
        ram_enabled = (byte & 0x0A) == 0x0A;
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
}

static inline void mbc2(u_int16 address, u_int8 byte)
{
    if (address <= 0x3FFF) {
        if (!is_set(address, 8)) {
            ram_enabled =(byte & 0x0A) == 0x0A;
        } else {
            bank = byte & 0x0F;
            if (!bank) {
                bank = 1;
            }
        }
    }
}

static inline void mbc3(u_int16 address, u_int8 byte)
{
    if (address >= 0 && address <= 0x1FFF) {
        ram_enabled =(byte & 0x0A) == 0x0A;
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        if (!byte) {
            byte = 1;
        }
        bank = byte & 0x7F;
    }
    else if (address >= 0x4000 && address <= 0x5FFF) {
        if (byte >= 0 && byte <= 0x3) {
            ram_bank = 3 & byte;
            rtc_select = 0;
        } else if (byte >= 0x08 && byte <= 0x0C) {
            rtc_select = 1;
            ram_bank = byte >> 3;
        }
    }
}

static inline void mbc5(u_int16 address, u_int8 byte)
{
    if (address >= 0 && address <= 0x1FFF) {
        ram_enabled = (byte & 0x0A) == 0x0A;
    }
    else if (address >= 0x2000 && address <= 0x2FFF) {
        bank = byte;
    }
    else if (address >= 0x3000 && address <= 0x3FFF) {
        bank = ((byte & 0x1) << 8) | (bank & 0xFF);
    }
    else if (address >= 0x4000 && address <= 0x5FFF) {
        ram_bank = byte & 0xf;
    }
    if (!bank) {
        bank = 1;
    }
}

static inline void write_memory(u_int16 address, u_int8 byte)
{
    if (debug && paused) {
        printf("WRITE: %x %x, BANK: %d\n", address, byte, bank);
        fflush(stdout);
    }
    if (address >= 0 && address <= 0x7FFF) {
        if (headers.mbc == 1) {
            mbc1(address, byte);
        }
        else if (headers.mbc == 2) {
            mbc2(address, byte);
        }
        else if (headers.mbc == 3) {
            mbc3(address, byte);
        }
        else if (headers.mbc == 5) {
            mbc5(address, byte);
        }
        return;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        vram[(address - 0x8000) + vram_bank * 0x2000] = byte;
        memory[address] = byte;
        return;
    }

    // External ram
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (!ram_enabled) {
            return;
        }
        if (headers.mbc == 2) {
            address &= 0x01FF;
        }
        cart_ram[(address - 0xA000) + (ram_bank * 0x2000)] = byte;
        return;
    }

    //echo
    if (address >= 0xC000 && address <= 0xDDFF) {
        memory[address + 0x2000] = byte;
    }

    // WRAM bank 0
    if (address >= 0xC000 && address <= 0xCFFF) {
        wram[address - 0xC000] = byte;
        return;
    }

    // WRAM bank 1-7
    if (address >= 0xD000 && address <= 0xDFFF) {  
        if (((address - 0xD000) + wram_bank * 0x1000) >= 32768) {
            printf("wram out of range\n");
        }
        wram[(address - 0xD000) + wram_bank * 0x1000] = byte;
        return;
    }

    if (address >= 0xFF10 && address <= 0xFF3F) {
        do_sound(address, byte);
        return;
    }
    // DMA transfer
    if (address == 0xff46) {
        do_oam_transfer(byte);
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

    if (address == 0xff51) {
        dma.source = ((byte << 8) | (memory[0xff52])) & 0xFFF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff52) {
        dma.source = ((memory[0xff51] << 8) | (byte)) & 0xFFF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff53) {
        dma.dest = ((byte << 8) | (memory[0xff54])) & 0x1FF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff54) {
        dma.dest = ((memory[0xff53] << 8) | (byte)) & 0x1FF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff55) {
        do_dma(byte);
        return;
    }

    if (address == 0xff68) {
        bg_palette.auto_inc = is_set(byte, 7);
        bg_palette.index = 0x3f & byte;
        memory[address] = byte;
        return;
    }

    if (address == 0xff69) {
        bg_palette.palette[bg_palette.index] = byte;
        if (bg_palette.auto_inc) {
            ++bg_palette.index;
            if (bg_palette.index > 0x3f) {
                bg_palette.index = 0;
            }
        }
        memory[address] = byte;
        return;
    }

    if (address == 0xff6A) {
        spr_palette.auto_inc = is_set(byte, 7);
        spr_palette.index = 0x3f & byte;
        memory[address] = byte;
        return;
    }

    if (address == 0xff6B) {
        spr_palette.palette[spr_palette.index] = byte;
        if (spr_palette.auto_inc) {
            spr_palette.index++;
            if (spr_palette.index > 0x3f) {
                spr_palette.index = 0;
            }
        }
        memory[address] = byte;
        return;
    }

    // unusable memory
    if (address >= 0xFEA0 && address <= 0xFEFF) {
        return;
    }

    if (address == 0xff4f && headers.cgb) {
        vram_bank = 1 & byte;
    }

    if (address == 0xff70 && headers.cgb) {
        wram_bank = byte & 7;
        if (!wram_bank) 
            wram_bank = 1;
    }

    if (headers.cgb && address == 0xFF4D) {
        if (is_set(byte, 0)) {
            printf("prepare speed %d\n", double_speed);
            prepare_speed = 1;
        }
        memory[address] = byte;
        return;
    }

    if (headers.cgb && address == 0xFF56) {
        printf("infrared not supported\n");
    }

    memory[address] = byte;
}

// transfer sprite data to oam memory
static inline void do_oam_transfer(u_int8 byte)
{
    int b = (int)byte << 8;
    int c = 0x00;
    for (int i = b; i < b + 0xA0; ++i) {
        write_memory(0xfe00+c, read_memory(i));
        ++c;
    }
}

static inline void do_dma(u_int8 byte)
{
    if (!is_set(byte, 7)) {
        if (dma.active) {
            dma.active = 0;
        } else {
            do_gdma(byte);
        }
    } else {
        dma.length = ((byte & 0x7f) + 1) * 0x10;
        dma.position = 0;
        dma.active = 1;
    }
}

static inline void do_gdma(u_int8 byte)
{
    int length = ((byte & 0x7F) + 1) * 0x10;
    for (int i = 0; i < length; ++i) {
        write_memory(dma.dest + 0x8000 + i, read_memory(dma.source + i));
    }
    memory[0xff55] = 0xff;
}

static inline void do_hdma()
{
    for (int i = 0; i < 0x10; ++i) {
        if (dma.dest + 0x8000 + i + dma.position > 0x9FFF) {
            dma.active = 0;
            memory[0xff55] = 0xff;
            return;
        }
        write_memory(dma.dest + 0x8000 + i + dma.position, read_memory(dma.source + i + dma.position));
        dma.length--;
    }
    dma.position += 0x10;
    if (dma.length == 0) {
        memory[0xff55] = 0xff;
        dma.active = 0;
    }
}

static inline void do_sound(u_int16 address, u_int8 byte)
{
    sync_sound(address, byte);
}

#endif // MEMORY_H