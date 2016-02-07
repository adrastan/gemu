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
extern union program_counter pc;

#endif // REGISTERS_H
