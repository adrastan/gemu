#ifndef CPU_H
#define CPU_H

#include "system_types.h"

// main registers
union Registers {
    struct {
        u16 AF, BC, DE, HL; // MSB -> LSB
    } word;
    struct {
        u8 F, A, C, B, E, D, L, H; // LSB -> MSB
    } byte;
};

// stack pointer
union StackPointer {
    u16 SP;
    struct {
        u8 L, H;
    } byte;
};

// program counter
union ProgramCounter {
    u16 PC;
    struct {
        u8 L, H;
    } byte;
};

class Cpu
{
    public:
    Cpu();

    Registers regs;
    ProgramCounter pc;
    StackPointer sp;

    int halt = 0;
    int stopped = 0;
    int prepare_speed = 0;
    int double_speed = 0;
    int ime = 0;

    u8 test_z(void);
    u8 test_n(void);
    u8 test_c(void);
    u8 test_h(void);
    void set_z(void);
    void reset_z(void);
    void set_n(void);
    void reset_n(void);
    void set_h(void);
    void reset_h(void);
    void set_c(void);
    void reset_c(void);
};

#endif //CPU_H
