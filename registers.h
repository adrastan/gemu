#ifndef REGISTERS_H
#define REGISTERS_H

#include "cpu_types.h"

// main registers
union registers {
    struct {
        u_int16 AF, BC, DE, HL; // MSB -> LSB
    } word;
    struct {
        u_int8 F, A, C, B, E, D, L, H; // LSB -> MSB
    } byte;
};

// stack pointer
union stack_pointer {
    u_int16 SP;
    struct {
        u_int8 L, H;
    } byte;
};

// program counter
union program_counter {
    u_int16 PC;
    struct {
        u_int8 L, H;
    } byte;
};

extern union registers regs;
extern union stack_pointer sp;
extern union program_counter pc;

#endif // REGISTERS_H
