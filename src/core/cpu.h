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
};

#endif //CPU_H