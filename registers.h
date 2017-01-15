<<<<<<< HEAD
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
=======
#ifndef REGISTERS_H
#define REGISTERS_H

// main registers
union registers {
    struct {
        WORD AF, BC, DE, HL; // MSB -> LSB
    } word;
    struct {
        BYTE F, A, C, B, E, D, L, H; // LSB -> MSB
    } byte;
};

// stack pointer
union stack_pointer {
    WORD SP;
    struct {
        BYTE L, H;
    } byte;
};

// program counter
union program_counter {
    WORD PC;
    struct {
        BYTE L, H;
    } byte;
};

extern union registers regs;
extern union stack_pointer sp;
>>>>>>> origin/master
extern union program_counter pc;
extern BYTE * const A;
extern BYTE * const F;
extern BYTE * const B;
extern BYTE * const C;
extern BYTE * const D;
extern BYTE * const E;
extern BYTE * const H;
extern BYTE * const L;
extern WORD * const AF;
extern WORD * const BC;
extern WORD * const DE;
extern WORD * const HL;

#endif // REGISTERS_H
