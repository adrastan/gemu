#ifndef MEMORY_H
#define MEMORY_H
#include "registers.h"
#include "lcd_controller.h"
#include "cpu.h"
#include <stdio.h>

#define MEM_SIZE 65536

extern u_int8 memory[];

static inline u_int8 read_memory(u_int16 address)
{
    return memory[address];
}

static inline void write_memory(u_int16 address, u_int8 byte)
{
    if (address == 0xff46) {
        int b = (int)byte << 8;
        int c = 0x00;
        for (int i = b; i < b + 0xA0; ++i) {
            memory[0xfe00+c] = memory[i];
            ++c;
        }
        return;
    }
    if (address == 0xff00) {
        memory[address] &= 0xCF;
        memory[address] |= byte;
        return;
    }
    if (address == 0xff40) {
        if (is_set(byte,7) && !is_set(memory[address],7)) {
            switch_mode(2);
        }
    }
    if (address == 0xff04) {
        memory[address] = 0;
        memory[address+1] = 0;
        return;
    }
    if (address < 0x8000) {
        return;
    }
    if (address >= 0xE000 && address < 0xFE00) {
        memory[address] = byte;
        memory[address-0x2000] = byte;
        return;
    }
    if (address >= 0xC000 && address < 0xDE00) {
        memory[address] = byte;
        memory[address+0x2000] = byte;
        return;
    }
    if (address >= 0xFEA0 && address <= 0xFEFF) {
        return;
    }
    memory[address] = byte;
}

#endif // MEMORY_H
