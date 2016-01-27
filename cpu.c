#include "cpu.h"
#include "opcodes.h"

#define INITIAL_PC 0x0100
#define INITIAL_SP 0xFFFE

/* bytes are read from memory from low -> high and stored
   in each register pair from high -> low. */
union {
    struct {
        WORD AF, BC, DE, HL; // MSB -> LSB
    } word;
    struct {
        BYTE F, A, C, B, E, D, L, H; // LSB -> MSB
    } byte;
} regs;

union {
    WORD SP;
    struct {
        BYTE L, H;
    } byte;
} stack;

BYTE opcode; //opcode
WORD pc; // program counter
BYTE memory[65536] = {0}; // 16-bit memory
int cycles[256] = {4,12,8,8,4,4,8,4,20,8,8,8,4,4,8,4,
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
int counter;

void start_cpu()
{
    pc = INITIAL_PC;
    stack.SP = INITIAL_SP;
    counter = 100;

    for (;;) {
        opcode = memory[pc++];
        counter -= cycles[opcode];
        switch (opcode) {
            case 0x00: break;
            case 0x01: opcode_01();
            case 0x02: opcode_02();
        }
    }
}

BYTE test_z(BYTE f)
{
    return f & 0x80;
}
