#include "memory.h"

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

void do_banking(u_int16 address, u_int8 byte)
{
    // RAM enable
    if (address >= 0 && address <= 0x1FFF) {
        if (MBC2) {
            // lsb of upper address byte must not be zero to enable/disable RAM
            if ((address & (1 << 8))) {
                return;
            }
        }
        if (MBC3) {
            if (byte == 0x0A) {
                ram_enabled = 1;
            }
            if (byte == 0) {
                ram_enabled = 0;
            }
            return;
        }
        byte &= 0x0F;
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
        if (MBC3) {
            bank = byte & 0x7F;
            if (bank == 0) {
                ++bank;
            }
            return;
        }
        u_int8 lower_5 = byte & 0x1F;
        bank &= 0xE0;
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
        if (MBC3) {
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
        byte &= 1;
        bank_mode = byte;
        if (bank_mode == 1) {
            bank &= 0x9F;
        }
        if (bank_mode == 0) {
            ram_bank = 0;
        }
        return;
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

