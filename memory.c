#include "memory.h"

u_int8 memory[MEM_SIZE] = {0}; // 16-bit memory
u_int8 cart_rom[4194304] = {0};
u_int8 cart_ram[32768] = {0};
u_int8 bank = 1;
u_int8 ram_bank = 0;
int ram_enabled = 0;
int bank_mode = 0;
int MBC2;

void do_banking(u_int16 address, u_int8 byte)
{
    // RAM enable
    if (address >= 0 && address <= 0x1FFF) {
        if (MBC2) {
            if ((address & (1 << 8)) != 0) {
                return;
            }
        }
        byte = byte & 0x0F;
        if (byte == 0x0A) {
            ram_enabled = 1;
        } else {
            ram_enabled = 0;
        }
        return;
    }
    // select rom bank lower
    if (address >= 0x2000 && address <= 0x3FFF) {
        if (MBC2) {
            if (address & (1 << 8)) {
                u_int8 lower_4 = byte & 0x0F;
                bank = 0;
                bank |= lower_4;
                if (bank == 0) {
                    ++bank;
                }
            }
            return;
        }
        u_int8 lower_5 = byte & 0x1F;
        bank = bank & 0xE0;
        bank |= lower_5;
        if (bank == 0 || bank == 0x20 || bank == 0x40 || bank == 0x60) {
            ++bank;
        }
        return;
    }
    // select rom bank upper
    if (address >= 0x4000 && address <= 0x5FFF) {
        if (MBC2) {
            return;
        }
        if (bank_mode == 0) {
            u_int8 upper_2 = byte & 0x60;
            bank = bank & 0x9F;
            bank |= (upper_2);
            if (bank == 0 || bank == 0x20 || bank == 0x40 || bank == 0x60) {
                ++bank;
            }
            return;
        } else {
            if (byte > 3) {
                return;
            }
            ram_bank = byte;
            return;
        }
    }
    // mode select
    if (address >= 0x6000 && address <= 0x7FFF) {
        if (MBC2) {
            return;
        }
        byte = byte & 1;
        bank_mode = byte;
        if (bank_mode == 1) {
            bank = bank & 0x9F;
        }
        if (bank_mode == 0) {
            ram_bank = 0;
        }
        return;
    }
}

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

