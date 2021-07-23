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

#include <stdlib.h>

#ifndef EMSCRIPTEN
#include <SDL2/SDL.h>
SDL_Event event;
#endif

#include "cpu.h"
#include "opcodes.h"
#include "memory.h"
#include "registers.h"
#include "lcd_controller.h"
#include "timers.h"
#include "interrupts.h"
#include "joypad.h"
#include "sound.h"

char * title = NULL; // title of the rom without extension
char * file_location = NULL; // absolute file location
char * save_file = NULL; // title with .SAVE extension
int debug;
int save_request;
int load_request;
int transfer_time;
int transfer;
int count;
int delay;
int halt;
int ime = 1; // interrupt master enable
int counter;
u_int8 opcode;
int op_cycles[256] = {4,12,8,8,4,4,8,4,20,8,8,8,4,4,8,4,
                     4,12,8,8,4,4,8,4,12,8,8,8,4,4,8,4,
                     12,12,8,8,4,4,8,4,12,8,8,8,4,4,8,4,
                     12,12,8,8,12,12,12,4,12,8,8,8,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     8,8,8,8,8,8,4,8,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     4,4,4,4,4,4,8,4,4,4,4,4,4,4,8,4,
                     20,12,16,16,24,16,8,16,20,16,16,4,24,24,8,16,
                     20,12,16,0,24,16,8,16,20,16,16,0,24,0,8,16,
                     12,12,8,0,0,16,8,16,16,4,16,0,0,0,8,16,
                     12,12,8,4,0,16,8,16,12,8,16,4,0,0,8,16};
int pre_cycles[256] = {8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8,
                       8,8,8,8,8,8,16,8,8,8,8,8,8,8,16,8};

//int clock = 4194304;
const double FRAMES_PER_SECOND = 60;
int cap, program_running;
double start_time, end_time;
int fps_count;

void start_cpu(char *game)
{
    printf("Starting cpu...\n");
    FILE *fp;
    if (file_location == NULL && game != NULL) {
        fp = fopen(game, "rb");
    } else {
        fp = fopen(file_location, "rb");
    }
    if (fp != NULL) {
        read_rom(fp);
    }
    
    initCart();
    init_regs();
    set_clock_freq();
    switch_mode(2);
    char c;
    char p[] = "0";
    int n = (int)strtol(p,NULL,16);
    program_running = 1;
    cap = 1;
    // start_main_loop();
}

void start_main_loop() {
    while (program_running) {
        get_next_frame();
    }
}

#ifndef EMSCRIPTEN
void get_next_frame() {
    start_time = SDL_GetTicks();
    while (fps_count <= 70224) {
        update_frame(&event);
    }
    draw_frame();
    end_time = SDL_GetTicks();
    if ((end_time - start_time) < 1000.0 / FRAMES_PER_SECOND) {
        if (cap) {
            SDL_Delay((1000.0 / FRAMES_PER_SECOND) - (end_time - start_time));
        }
    }
    update_sound(counter);
    fps_count -= 70224;
}

void update_frame(SDL_Event *event)
{
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: update_joypad(&event->key); break;
            case SDL_QUIT: program_running = 0; break;
        }
    }

    check_interrupts();
    counter = delay;
    delay = 0;
    if (!halt) {
        opcode = read_memory(pc.PC++);
        counter += op_cycles[opcode];
        process_opcode();
    } else {
        counter += 4;
    }
    fps_count += counter;
    update_lcd(counter);
    update_timers(counter);
    update_serial(counter);
}
#endif // ifndef EMSCRIPTEN

#ifdef EMSCRIPTEN
void get_next_frame() {
    while (fps_count <= 70224) {
        update_frame();
    }
    fps_count -= 70224;
}

void update_frame()
{
    check_interrupts();
    counter = delay;
    delay = 0;
    if (!halt) {
        opcode = read_memory(pc.PC++);
        counter += op_cycles[opcode];
        process_opcode();
    } else {
        counter += 4;
    }
    fps_count += counter;
    update_lcd(counter);
    update_timers(counter);
    update_serial(counter);
    update_sound(counter);
}
#endif

void initCart()
{
    ram_enabled = memory[0x0149];
    headers.mbc = memory[0x147];
    for (int i = 0; i < 48; ++i) {
        headers.logo[i] = memory[0x104 + i];
    }
    for (int i = 0; i < 16; ++i) {
        headers.title[i] = memory[0x134 + i];
    }
    headers.cgb = is_set(memory[0x143], 7);
    headers.sgb = memory[0x146] == 0x03;
    headers.rom_size = memory[0x148];
    headers.ram_size = memory[0x149];
    printf("mbc %d\n", headers.mbc);
    if ((headers.mbc >= 0x08 && headers.mbc <= 0x0d) || headers.mbc > 0x13) {
        printf("mbc %d not supported\n", headers.mbc);
        exit(1);
    }
}

void update_serial(int cycles)
{
    if (!is_set(memory[0xff02], 7) || !is_set(memory[0xff02], 0)) {
        return;
    }
    transfer_time += counter;
    if (transfer_time >= 4096) {
        request_interrupt(3);
        transfer_time = 0;
        transfer = 0;
        memory[0xff01] = 0xff;
        memory[0xff02] &= ~(1 << 7);
    }
}

void set_delay(int d)
{
    delay = d;
}

// returns 1 if flag z is set
u_int8 test_z()
{
    return (regs.byte.F & 0x80) >> 7;
}
// set z flag
void set_z()
{
    regs.byte.F |= 1 << 7;
}
// reset z flag
void reset_z()
{
    regs.byte.F &= ~(1 << 7);
}
// returns 1 if flag n is set
u_int8 test_n()
{
    return (regs.byte.F & 0x40) >> 6;
}
// set n flag
void set_n()
{
    regs.byte.F |= 1 << 6;
}
// reset n flag
void reset_n()
{
    regs.byte.F &= ~(1 << 6);
}
// returns 1 if flag h is set
u_int8 test_h()
{
    return (regs.byte.F & 0x20) >> 5;
}
// set h flag
void set_h()
{
    regs.byte.F |= 1 << 5;
}
// reset h flag
void reset_h()
{
    regs.byte.F &= ~(1 << 5);
}
// returns 1 if flag c is set
u_int8 test_c()
{
    return (regs.byte.F & 0x10) >> 4;
}
// set c flag
void set_c()
{
    regs.byte.F |= 1 << 4;
}
// reset c flag
void reset_c()
{
    regs.byte.F &= ~(1 << 4);
}

// Returns 1 if adding bytes a and b results in half carry.
int half_carry(u_int8 a, u_int8 b)
{
    return (((a & 0xf) + (b & 0xf)) & 0x10) == 0x10;
}

int is_set(u_int8 byte, int bit)
{
    return byte & (1 << bit);
}

void set_bit(u_int8 *byte, int bit)
{
    *byte |= (1 << bit);
}

void reset_bit(u_int8 *byte, int bit)
{
    *byte &= ~(1 << bit);
}

// initializes registers
void init_regs()
{
    pc.PC = 0x0100;
    sp.SP = 0xFFFE;
    regs.word.AF = 0x01B0;
    regs.word.BC = 0x0013;
    regs.word.DE = 0x00D8;
    regs.word.HL = 0x014D;
    write_memory(0xFF05, 0x00);
    write_memory(0xFF06, 0x00);
    write_memory(0xFF07, 0x00);
    init_sound_regs();
    init_wave_ram();
    write_memory(0xFF40, 0x91);
    write_memory(0xFF42, 0x00);
    write_memory(0xFF43, 0x00);
    write_memory(0xFF45, 0x00);
    write_memory(0xFF47, 0xFC);
    write_memory(0xFF48, 0xFF);
    write_memory(0xFF49, 0xFF);
    write_memory(0xFF4A, 0x00);
    write_memory(0xFF4B, 0x00);
    write_memory(0xFFFF, 0x00);
    memory[0xFF00] = 0xFF;
    memory[0xFF02] = 0x7C;
}

// reads the contents of rom file to memory
void read_rom(FILE* fp)
{
    fseek(fp,0L,SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    u_int8 arr[size];
    int n = fread(arr,1,size,fp);
    if (n < 0) {
        exit(0);
    }
    int j = 0;
    for (int i = 0; i < 32768; ++i, ++j) {
        memory[j] = arr[i];
    }
    j = 0;
    for (int i = 0; i < size; ++i, ++j) {
        cart_rom[j] = arr[i];
    }
    fclose(fp);
}

void push(u_int16 word)
{
    u_int8 high = (word & 0xff00) >> 8;
    u_int8 low = (word & 0x00ff);
    write_memory(sp.SP-1, high);
    write_memory(sp.SP-2, low);
    sp.SP = sp.SP - 2;
}

void print_flags()
{
    printf("AF: %.4x\n", regs.word.AF);
    printf("BC: %.4hx\n", regs.word.BC);
    printf("DE: %.4hx\n", regs.word.DE);
    printf("HL: %.4hx\n", regs.word.HL);
    printf("SP: %.4hx\n", sp.SP);
    printf("PC: %.4hx\n", pc.PC);
    printf("Z: %d\n", test_z());
    printf("N: %d\n", test_n());
    printf("H: %d\n", test_h());
    printf("C: %d\n", test_c());
    printf("ffff: %hx\n", memory[0xffff]);
    printf("ff0f: %hx\n", memory[0xff0f]);
    printf("ime: %d\n", ime);
    printf("ly: %d\n", memory[0xff44]);
    printf("ff40: %hx\n", memory[0xff40]);
    printf("ff01: %hx\n", memory[0xff01]);
    printf("ff02: %hx\n", memory[0xff02]);
    printf("9dc1: %d\n", memory[0x9dc1]);
}

// save game to file
void save_state()
{
    FILE *st = fopen(save_file, "wb");
    u_int8 *bytes = get_state();
    fwrite(bytes,1,0x10000+20,st);
    fflush(st);
    fclose(st);
    save_request = 0;
}

// load game save file
void load_state()
{
    FILE *ls = fopen(save_file, "rb");
    if (ls == NULL) {
        return;
    }
    u_int8 bytes[0x10000+20];
    fread(bytes,1,0x10000+20,ls);
    for (int i = 0; i < 0x8000; ++i) {
        memory[0x8000+i] = bytes[i];
    }
    for (int i = 0x8000; i < 0x10000; ++i) {
        cart_ram[i-0x8000] = bytes[i];
    }
    pc.byte.L = bytes[0x10000];
    pc.byte.H = bytes[0x10001];
    sp.byte.L = bytes[0x10002];
    sp.byte.H = bytes[0x10003];
    regs.byte.A = bytes[0x10004];
    regs.byte.F = bytes[0x10005];
    regs.byte.B = bytes[0x10006];
    regs.byte.C = bytes[0x10007];
    regs.byte.D = bytes[0x10008];
    regs.byte.E = bytes[0x10009];
    regs.byte.H = bytes[0x1000A];
    regs.byte.L = bytes[0x1000B];
    halt = bytes[0x1000C];
    ime = bytes[0x1000D];
    bank = bytes[0x1000E];
    ram_bank = bytes[0x1000F];
    ram_enabled = bytes[0x10010];
    bank_mode = bytes[0x10011];
    fclose(ls);
    interrupt_cycles[0] = 204;
    interrupt_cycles[1] = 80;
    interrupt_cycles[2] = 172;
    load_request = 0;
}

// returns a pointer to the current state of the emulator
u_int8* get_state()
{
    static u_int8 bytes[0x10000+20];

    for (int i = 0; i < 0x8000; ++i) {
        bytes[i] = memory[0x8000+i];
    }
    for (int i = 0x8000; i < 0x10000; ++i) {
        bytes[i] = cart_ram[i-0x8000];
    }

    bytes[0x10000] = pc.byte.L;
    bytes[0x10001] = pc.byte.H;
    bytes[0x10002] = sp.byte.L;
    bytes[0x10003] = sp.byte.H;
    bytes[0x10004] = regs.byte.A;
    bytes[0x10005] = regs.byte.F;
    bytes[0x10006] = regs.byte.B;
    bytes[0x10007] = regs.byte.C;
    bytes[0x10008] = regs.byte.D;
    bytes[0x10009] = regs.byte.E;
    bytes[0x1000A] = regs.byte.H;
    bytes[0x1000B] = regs.byte.L;
    bytes[0x1000C] = halt;
    bytes[0x1000D] = ime;
    bytes[0x1000E] = bank;
    bytes[0x1000F] = ram_bank;
    bytes[0x10010] = ram_enabled;
    bytes[0x10011] = bank_mode;

    return bytes;
}

// resets the emulator
void restart()
{
    for (int i = 0x8000; i < 0x10000; ++i) {
        memory[i] = 0;
        cart_ram[i-0x8000] = 0;
    }
    set_clock_freq();
    switch_mode(2);
    init_regs();
    debug = 0;
    save_request = 0;
    load_request = 0;
    transfer_time = 0;
    transfer = 0;
    count = 0;
    delay = 0;
    halt = 0;
    fps_count = 0;
    ime = 1;
    counter = 0;
    interrupt_cycles[0] = 204;
    interrupt_cycles[1] = 80;
    interrupt_cycles[2] = 172;
    interrupt_cycles[3] = 456;
}

void process_opcode()
{
    switch (opcode) {
        case 0x00: break;
        case 0x01: opcode_01(); break;
        case 0x02: opcode_02(); break;
        case 0x03: opcode_03(); break;
        case 0x04: opcode_04(); break;
        case 0x05: opcode_05(); break;
        case 0x06: opcode_06(); break;
        case 0x07: opcode_07(); break;
        case 0x08: opcode_08(); break;
        case 0x09: opcode_09(); break;
        case 0x0A: opcode_0A(); break;
        case 0x0B: opcode_0B(); break;
        case 0x0C: opcode_0C(); break;
        case 0x0D: opcode_0D(); break;
        case 0x0E: opcode_0E(); break;
        case 0x0F: opcode_0F(); break;
        case 0x10: opcode_10(); break;
        case 0x11: opcode_11(); break;
        case 0x12: opcode_12(); break;
        case 0x13: opcode_13(); break;
        case 0x14: opcode_14(); break;
        case 0x15: opcode_15(); break;
        case 0x16: opcode_16(); break;
        case 0x17: opcode_17(); break;
        case 0x18: opcode_18(); break;
        case 0x19: opcode_19(); break;
        case 0x1A: opcode_1A(); break;
        case 0x1B: opcode_1B(); break;
        case 0x1C: opcode_1C(); break;
        case 0x1D: opcode_1D(); break;
        case 0x1E: opcode_1E(); break;
        case 0x1F: opcode_1F(); break;
        case 0x20: opcode_20(); break;
        case 0x21: opcode_21(); break;
        case 0x22: opcode_22(); break;
        case 0x23: opcode_23(); break;
        case 0x24: opcode_24(); break;
        case 0x25: opcode_25(); break;
        case 0x26: opcode_26(); break;
        case 0x27: opcode_27(); break;
        case 0x28: opcode_28(); break;
        case 0x29: opcode_29(); break;
        case 0x2A: opcode_2A(); break;
        case 0x2B: opcode_2B(); break;
        case 0x2C: opcode_2C(); break;
        case 0x2D: opcode_2D(); break;
        case 0x2E: opcode_2E(); break;
        case 0x2F: opcode_2F(); break;
        case 0x30: opcode_30(); break;
        case 0x31: opcode_31(); break;
        case 0x32: opcode_32(); break;
        case 0x33: opcode_33(); break;
        case 0x34: opcode_34(); break;
        case 0x35: opcode_35(); break;
        case 0x36: opcode_36(); break;
        case 0x37: opcode_37(); break;
        case 0x38: opcode_38(); break;
        case 0x39: opcode_39(); break;
        case 0x3A: opcode_3A(); break;
        case 0x3B: opcode_3B(); break;
        case 0x3C: opcode_3C(); break;
        case 0x3D: opcode_3D(); break;
        case 0x3E: opcode_3E(); break;
        case 0x3F: opcode_3F(); break;
        case 0x40: opcode_40(); break;
        case 0x41: opcode_41(); break;
        case 0x42: opcode_42(); break;
        case 0x43: opcode_43(); break;
        case 0x44: opcode_44(); break;
        case 0x45: opcode_45(); break;
        case 0x46: opcode_46(); break;
        case 0x47: opcode_47(); break;
        case 0x48: opcode_48(); break;
        case 0x49: opcode_49(); break;
        case 0x4A: opcode_4A(); break;
        case 0x4B: opcode_4B(); break;
        case 0x4C: opcode_4C(); break;
        case 0x4D: opcode_4D(); break;
        case 0x4E: opcode_4E(); break;
        case 0x4F: opcode_4F(); break;
        case 0x50: opcode_50(); break;
        case 0x51: opcode_51(); break;
        case 0x52: opcode_52(); break;
        case 0x53: opcode_53(); break;
        case 0x54: opcode_54(); break;
        case 0x55: opcode_55(); break;
        case 0x56: opcode_56(); break;
        case 0x57: opcode_57(); break;
        case 0x58: opcode_58(); break;
        case 0x59: opcode_59(); break;
        case 0x5A: opcode_5A(); break;
        case 0x5B: opcode_5B(); break;
        case 0x5C: opcode_5C(); break;
        case 0x5D: opcode_5D(); break;
        case 0x5E: opcode_5E(); break;
        case 0x5F: opcode_5F(); break;
        case 0x60: opcode_60(); break;
        case 0x61: opcode_61(); break;
        case 0x62: opcode_62(); break;
        case 0x63: opcode_63(); break;
        case 0x64: opcode_64(); break;
        case 0x65: opcode_65(); break;
        case 0x66: opcode_66(); break;
        case 0x67: opcode_67(); break;
        case 0x68: opcode_68(); break;
        case 0x69: opcode_69(); break;
        case 0x6A: opcode_6A(); break;
        case 0x6B: opcode_6B(); break;
        case 0x6C: opcode_6C(); break;
        case 0x6D: opcode_6D(); break;
        case 0x6E: opcode_6E(); break;
        case 0x6F: opcode_6F(); break;
        case 0x70: opcode_70(); break;
        case 0x71: opcode_71(); break;
        case 0x72: opcode_72(); break;
        case 0x73: opcode_73(); break;
        case 0x74: opcode_74(); break;
        case 0x75: opcode_75(); break;
        case 0x76: opcode_76(); break;
        case 0x77: opcode_77(); break;
        case 0x78: opcode_78(); break;
        case 0x79: opcode_79(); break;
        case 0x7A: opcode_7A(); break;
        case 0x7B: opcode_7B(); break;
        case 0x7C: opcode_7C(); break;
        case 0x7D: opcode_7D(); break;
        case 0x7E: opcode_7E(); break;
        case 0x7F: opcode_7F(); break;
        case 0x80: opcode_80(); break;
        case 0x81: opcode_81(); break;
        case 0x82: opcode_82(); break;
        case 0x83: opcode_83(); break;
        case 0x84: opcode_84(); break;
        case 0x85: opcode_85(); break;
        case 0x86: opcode_86(); break;
        case 0x87: opcode_87(); break;
        case 0x88: opcode_88(); break;
        case 0x89: opcode_89(); break;
        case 0x8A: opcode_8A(); break;
        case 0x8B: opcode_8B(); break;
        case 0x8C: opcode_8C(); break;
        case 0x8D: opcode_8D(); break;
        case 0x8E: opcode_8E(); break;
        case 0x8F: opcode_8F(); break;
        case 0x90: opcode_90(); break;
        case 0x91: opcode_91(); break;
        case 0x92: opcode_92(); break;
        case 0x93: opcode_93(); break;
        case 0x94: opcode_94(); break;
        case 0x95: opcode_95(); break;
        case 0x96: opcode_96(); break;
        case 0x97: opcode_97(); break;
        case 0x98: opcode_98(); break;
        case 0x99: opcode_99(); break;
        case 0x9A: opcode_9A(); break;
        case 0x9B: opcode_9B(); break;
        case 0x9C: opcode_9C(); break;
        case 0x9D: opcode_9D(); break;
        case 0x9E: opcode_9E(); break;
        case 0x9F: opcode_9F(); break;
        case 0xA0: opcode_A0(); break;
        case 0xA1: opcode_A1(); break;
        case 0xA2: opcode_A2(); break;
        case 0xA3: opcode_A3(); break;
        case 0xA4: opcode_A4(); break;
        case 0xA5: opcode_A5(); break;
        case 0xA6: opcode_A6(); break;
        case 0xA7: opcode_A7(); break;
        case 0xA8: opcode_A8(); break;
        case 0xA9: opcode_A9(); break;
        case 0xAA: opcode_AA(); break;
        case 0xAB: opcode_AB(); break;
        case 0xAC: opcode_AC(); break;
        case 0xAD: opcode_AD(); break;
        case 0xAE: opcode_AE(); break;
        case 0xAF: opcode_AF(); break;
        case 0xB0: opcode_B0(); break;
        case 0xB1: opcode_B1(); break;
        case 0xB2: opcode_B2(); break;
        case 0xB3: opcode_B3(); break;
        case 0xB4: opcode_B4(); break;
        case 0xB5: opcode_B5(); break;
        case 0xB6: opcode_B6(); break;
        case 0xB7: opcode_B7(); break;
        case 0xB8: opcode_B8(); break;
        case 0xB9: opcode_B9(); break;
        case 0xBA: opcode_BA(); break;
        case 0xBB: opcode_BB(); break;
        case 0xBC: opcode_BC(); break;
        case 0xBD: opcode_BD(); break;
        case 0xBE: opcode_BE(); break;
        case 0xBF: opcode_BF(); break;
        case 0xC0: opcode_C0(); break;
        case 0xC1: opcode_C1(); break;
        case 0xC2: opcode_C2(); break;
        case 0xC3: opcode_C3(); break;
        case 0xC4: opcode_C4(); break;
        case 0xC5: opcode_C5(); break;
        case 0xC6: opcode_C6(); break;
        case 0xC7: opcode_C7(); break;
        case 0xC8: opcode_C8(); break;
        case 0xC9: opcode_C9(); break;
        case 0xCA: opcode_CA(); break;
        case 0xCB: opcode_CB(); break;
        case 0xCC: opcode_CC(); break;
        case 0xCD: opcode_CD(); break;
        case 0xCE: opcode_CE(); break;
        case 0xCF: opcode_CF(); break;
        case 0xD0: opcode_D0(); break;
        case 0xD1: opcode_D1(); break;
        case 0xD2: opcode_D2(); break;
        case 0xD4: opcode_D4(); break;
        case 0xD5: opcode_D5(); break;
        case 0xD6: opcode_D6(); break;
        case 0xD7: opcode_D7(); break;
        case 0xD8: opcode_D8(); break;
        case 0xD9: opcode_D9(); break;
        case 0xDA: opcode_DA(); break;
        case 0xDC: opcode_DC(); break;
        case 0xDE: opcode_DE(); break;
        case 0xDF: opcode_DF(); break;
        case 0xE0: opcode_E0(); break;
        case 0xE1: opcode_E1(); break;
        case 0xE2: opcode_E2(); break;
        case 0xE5: opcode_E5(); break;
        case 0xE6: opcode_E6(); break;
        case 0xE7: opcode_E7(); break;
        case 0xE8: opcode_E8(); break;
        case 0xE9: opcode_E9(); break;
        case 0xEA: opcode_EA(); break;
        case 0xEE: opcode_EE(); break;
        case 0xEF: opcode_EF(); break;
        case 0xF0: opcode_F0(); break;
        case 0xF1: opcode_F1(); break;
        case 0xF2: opcode_F2(); break;
        case 0xF3: opcode_F3(); break;
        case 0xF5: opcode_F5(); break;
        case 0xF6: opcode_F6(); break;
        case 0xF7: opcode_F7(); break;
        case 0xF8: opcode_F8(); break;
        case 0xF9: opcode_F9(); break;
        case 0xFA: opcode_FA(); break;
        case 0xFB: opcode_FB(); break;
        case 0xFE: opcode_FE(); break;
        case 0xFF: opcode_FF(); break;
        default: printf("invalid opcode");
        return;
    }
}