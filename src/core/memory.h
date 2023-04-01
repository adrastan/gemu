#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 0x10000

#include "system_types.h"
#include "cartridge.h"
#include "lcd_controller.h"

typedef struct {
    u16 source;
    u16 dest;
    int length;
    int active;
    int position;
} DMA;

class LCDController;

class Memory
{
    public:
    Memory();

    u8 memory[MEM_SIZE];
    u8 vram[16384];
    u8 wram[32768];
    int bank;
    int ram_bank;
    int vram_bank;
    int wram_bank;
    int ram_enabled;
    int bank_mode;
    int rtc_select;
    int prepare_speed = 0;
    Cartridge *cart;
    LCDController *lcd_controller;
    DMA dma;

    u8 read_memory(u16);
    void write_memory(u16, u8);
    void mbc1(u16, u8);
    void mbc2(u16, u8);
    void mbc3(u16, u8);
    void mbc5(u16, u8);
    void do_oam_transfer(u8);
    void do_dma(u8);
    void do_gdma(u8);
    void do_hdma();
    u8 joypad_state();
};

#endif //MEMORY_H
