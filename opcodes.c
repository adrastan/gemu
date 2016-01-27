#include "cpu.h"
#include "opcodes.h"
#define SET_Z(F) F |= 1 << 7
#define RESET_Z(F) F &= ~(1 << 7)
#define SET_N(F) F |= 1 << 6
#define RESET_N(F) F &= ~(1 << 6)
#define SET_H(F) F |= 1 << 5
#define RESET_H(F) F &= ~(1 << 5)
#define SET_C(F) F |= 1 << 4
#define RESET_C(F) F &= ~(1 << 4)

/* bytes are read from memory from low -> high and stored
   in each register pair from high -> low. */
extern union {
    struct {
        WORD AF, BC, DE, HL; // MSB -> LSB
    } word;
    struct {
        BYTE F, A, C, B, E, D, L, H; // LSB -> MSB
    } byte;
} regs;

extern union {
    WORD SP;
    struct {
        BYTE L, H;
    } byte;
} stack;

extern BYTE memory[];
extern WORD pc;
extern int counter;

// LD BC, d16
void opcode_01()
{
    regs.byte.C = memory[pc++];
    regs.byte.B = memory[pc++];
}

// LD (BC), A
void opcode_02()
{
    memory[regs.word.BC] = regs.byte.A;
}

// INC BC
void opcode_03()
{
    ++regs.word.BC;
}

// INC B
void opcode_04()
{
    if (((0x01 & regs.byte.B) == 0x01) && ((0x02 & regs.byte.B) == 0x02) && ((0x04 & regs.byte.B) == 0x04) && ((0x08 & regs.byte.B) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.B;
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC B
void opcode_05()
{
    if (((0x01 & regs.byte.B) == 0x00) && ((0x02 & regs.byte.B) == 0x00) && ((0x04 & regs.byte.B) == 0x00) && ((0x08 & regs.byte.B) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.B;
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD B, d8
void opcode_06()
{
    regs.byte.B = memory[pc++];
}

// RLCA
void opcode_07()
{
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F); // set Z flag
        RESET_N(regs.byte.F); // reset N flag
        RESET_H(regs.byte.F); // reset H flag
        RESET_C(regs.byte.F); // reset C flag
        return;
    }
    BYTE bit = (regs.byte.A >> 7) & 1; // get 7th bit
    regs.byte.A = regs.byte.A << 1; // shift left 1
    regs.byte.A ^= (-bit ^ regs.byte.A) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// LD (a16), SP
void opcode_08()
{
    WORD lower = memory[pc++];
    WORD higher = memory[pc++];
    WORD address = (higher < 8) | lower;
    memory[address++] = stack.byte.L;
    memory[address] = stack.byte.H;
}

// ADD HL, BC
void opcode_09()
{
    RESET_N(regs.byte.F);
    if ((((regs.word.HL & 0x0FFF) + (regs.word.BC & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.word.HL >= regs.word.BC) {
        if ((regs.word.HL + regs.word.BC) >= regs.word.HL) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.HL + regs.word.BC) < regs.word.HL) {
            SET_C(regs.byte.F);
        }
    } else {
        if ((regs.word.BC + regs.word.HL) >= regs.word.BC) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.BC + regs.word.HL) < regs.word.BC) {
            SET_C(regs.byte.F);
        }
    }
    regs.word.HL += regs.word.BC;
}

// LD A, (BC)
void opcode_0A()
{
    regs.byte.A = memory[regs.word.BC];
}

// DEC BC
void opcode_0B()
{
    --regs.word.BC;
}

// INC C
void opcode_0C()
{
    if (((0x01 & regs.byte.C) == 0x01) && ((0x02 & regs.byte.C) == 0x02) && ((0x04 & regs.byte.C) == 0x04) && ((0x08 & regs.byte.C) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.C;
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC C
void opcode_0D()
{
    if (((0x01 & regs.byte.C) == 0x00) && ((0x02 & regs.byte.C) == 0x00) && ((0x04 & regs.byte.C) == 0x00) && ((0x08 & regs.byte.C) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.B;
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD C, d8
void opcode_0E()
{
    regs.byte.C = memory[pc++];
}

// RRCA
void opcode_0F()
{
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F); // set Z flag
        RESET_N(regs.byte.F); // reset N flag
        RESET_H(regs.byte.F); // reset H flag
        RESET_C(regs.byte.F); // reset C flag
        return;
    }
    BYTE bit = (regs.byte.A >> 0) & 1; // get 0th bit
    regs.byte.A = regs.byte.A >> 1; // shift right 1
    regs.byte.A ^= (-bit ^ regs.byte.A) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// STOP 0
void opcode_10()
{
    return;
}

// LD DE, d16
void opcode_11()
{
    regs.byte.E = memory[pc++];
    regs.byte.D = memory[pc++];
}

// LD (DE), A
void opcode_12()
{
    memory[regs.word.DE] = regs.byte.A;
}

// INC DE
void opcode_13()
{
    ++regs.word.DE;
}

// INC D
void opcode_14()
{
    if (((0x01 & regs.byte.D) == 0x01) && ((0x02 & regs.byte.D) == 0x02) && ((0x04 & regs.byte.D) == 0x04) && ((0x08 & regs.byte.D) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.D;
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC D
void opcode_15()
{
    if (((0x01 & regs.byte.D) == 0x00) && ((0x02 & regs.byte.D) == 0x00) && ((0x04 & regs.byte.D) == 0x00) && ((0x08 & regs.byte.D) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.D;
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD D, d8
void opcode_16()
{
    regs.byte.D = memory[pc++];
}

// RLA
void opcode_17()
{
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F); // set Z flag
        RESET_N(regs.byte.F); // reset N flag
        RESET_H(regs.byte.F); // reset H flag
        RESET_C(regs.byte.F); // reset C flag
        return;
    }
    BYTE bit = (regs.byte.A >> 7) & 1; // get 7th bit
    regs.byte.A = regs.byte.A << 1; // shift left 1
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// JR r8
void opcode_18()
{
    signed char byte = memory[pc++];
    pc += byte;
}

// ADD HL, DE
void opcode_19()
{
    RESET_N(regs.byte.F);
    if ((((regs.word.HL & 0x0FFF) + (regs.word.DE & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.word.HL >= regs.word.DE) {
        if ((regs.word.HL + regs.word.DE) >= regs.word.HL) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.HL + regs.word.DE) < regs.word.HL) {
            SET_C(regs.byte.F);
        }
    } else {
        if ((regs.word.DE + regs.word.HL) >= regs.word.DE) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.DE + regs.word.HL) < regs.word.DE) {
            SET_C(regs.byte.F);
        }
    }
    regs.word.HL += regs.word.DE;
}

// LD A, (DE)
void opcode_1A()
{
    regs.byte.A = memory[regs.word.DE];
}

// DEC DE
void opcode_1B()
{
    --regs.word.DE;
}

// INC E
void opcode_1C()
{
    if (((0x01 & regs.byte.E) == 0x01) && ((0x02 & regs.byte.E) == 0x02) && ((0x04 & regs.byte.E) == 0x04) && ((0x08 & regs.byte.E) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.E;
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC E
void opcode_1D()
{
    if (((0x01 & regs.byte.E) == 0x00) && ((0x02 & regs.byte.E) == 0x00) && ((0x04 & regs.byte.E) == 0x00) && ((0x08 & regs.byte.E) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.E;
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD E, d8
void opcode_1E()
{
    regs.byte.E = memory[pc++];
}

// RRA
void opcode_1F()
{
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F); // set Z flag
        RESET_N(regs.byte.F); // reset N flag
        RESET_H(regs.byte.F); // reset H flag
        RESET_C(regs.byte.F); // reset C flag
        return;
    }
    BYTE bit = (regs.byte.A >> 0) & 1; // get 0th bit
    regs.byte.A = regs.byte.A >> 1; // shift left 1
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// JR NZ, r8
void opcode_20()
{
    if (test_z(regs.byte.F)) {
        signed char byte = memory[pc++];
        pc += byte;
    } else {
        counter += 4;
    }
}

// LD HL, d16
void opcode_21()
{

}
