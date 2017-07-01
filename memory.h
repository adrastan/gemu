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

static inline u_int8 read_memory(u_int16 address)
{
    // returns byte depending on rom bank
    if (address >= 0x4000 && address <= 0x7FFF) {
        return cart_rom[(address - 0x4000) + (bank * 0x4000)];
    }
    // returns ram byte depending on ram bank
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (ram_enabled) {
            // using mbc2
            if (MBC2) {
                return cart_ram[(address - 0xA000)] & 0x0F;
            }
            // using mbc3
            if (MBC3) {
                if (ram_bank <= 3) {
                    return cart_ram[(address - 0xA000) + (ram_bank * 8192)];
                }
                return get_rtc();
            }
            return cart_ram[(address - 0xA000) + (ram_bank * 8192)];
        }
        return 0xFF;
    }
    if (address == 0xff00) {
        return joypad_state();
    }
    return memory[address];
}

static inline void write_memory(u_int16 address, u_int8 byte)
{
    // DMA transfer
    if (address == 0xff46) {
        do_dma(byte);
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
    }
    // reset timers
    if (address == 0xff04) {
        memory[address] = 0;
        memory[address+1] = 0;
        return;
    }
    // banking control
    if (address < 0x8000) {
        do_banking(address, byte);
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
    // ram area
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (ram_enabled) {
            if (ram_bank <= 3) {
                cart_ram[(address - 0xA000) + (ram_bank * 8192)] = byte;
            } else {
                write_rtc(ram_bank, byte);
            }
        }
        return;
    }
    memory[address] = byte;
}

#endif // MEMORY_H
