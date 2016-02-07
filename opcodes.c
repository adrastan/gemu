#include "cpu.h"
#include "opcodes.h"
#include "pre_opcodes.h"
#include "memory.h"
#include "registers.h"

extern int counter;
extern int enable_interrupt;
extern BYTE opcode;

// LD BC, d16
void opcode_01()
{
    regs.byte.C = memory[pc.PC++];
    regs.byte.B = memory[pc.PC++];
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
    regs.byte.B = memory[pc.PC++];
}

// RLCA
void opcode_07()
{
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
    WORD lower = memory[pc.PC++];
    WORD higher = memory[pc.PC++];
    WORD address = (higher < 8) | lower;
    memory[address++] = sp.byte.L;
    memory[address] = sp.byte.H;
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
    regs.byte.C = memory[pc.PC++];
}

// RRCA
void opcode_0F()
{
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
    regs.byte.E = memory[pc.PC++];
    regs.byte.D = memory[pc.PC++];
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
    regs.byte.D = memory[pc.PC++];
}

// RLA
void opcode_17()
{
    BYTE bit = (regs.byte.A >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.A = regs.byte.A << 1; // shift left 1
    regs.byte.A ^= (-carry ^ regs.byte.A) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// JR r8
void opcode_18()
{
    signed char byte = memory[pc.PC++];
    pc.PC += byte;
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
    regs.byte.E = memory[pc.PC++];
}

// RRA
void opcode_1F()
{
    BYTE bit = (regs.byte.A >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.A = regs.byte.A >> 1; // shift left 1
    regs.byte.A ^= (-carry ^ regs.byte.A) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// JR NZ, r8
void opcode_20()
{
    if (test_z(regs.byte.F) == 0) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// LD HL, d16
void opcode_21()
{
    regs.byte.L = memory[pc.PC++];
    regs.byte.H = memory[pc.PC++];
}

// LD (HL+), A
void opcode_22()
{
    memory[regs.word.HL] = regs.byte.A;
    ++regs.word.HL;
}

// INC HL
void opcode_23()
{
    ++regs.word.HL;
}

// INC H
void opcode_24()
{
    if (((0x01 & regs.byte.H) == 0x01) && ((0x02 & regs.byte.H) == 0x02) && ((0x04 & regs.byte.H) == 0x04) && ((0x08 & regs.byte.H) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.H;
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC H
void opcode_25()
{
    if (((0x01 & regs.byte.H) == 0x00) && ((0x02 & regs.byte.H) == 0x00) && ((0x04 & regs.byte.H) == 0x00) && ((0x08 & regs.byte.H) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.H;
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD H, d8
void opcode_26()
{
    regs.byte.H = memory[pc.PC++];
}

// DAA
void opcode_27()
{
    BYTE left_nibble = 0xF0 & regs.byte.A;
    left_nibble = left_nibble >> 4;
    BYTE right_nibble = 0x0F & regs.byte.A;

    if (test_n(regs.byte.F) == 0) {
        if (test_c(regs.byte.F) == 0) {
            if (left_nibble >= 0x0 && left_nibble <= 0x9 && test_h(regs.byte.F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                regs.byte.A += 0x00;
                RESET_C(regs.byte.F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x8 && test_h(regs.byte.F) == 0 && right_nibble >= 0xA && right_nibble <= 0xF) {
                regs.byte.A += 0x06;
                RESET_C(regs.byte.F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x9 && test_h(regs.byte.F) && right_nibble >= 0x0 && right_nibble <= 0x3) {
                regs.byte.A += 0x06;
                RESET_C(regs.byte.F);
            } else if (left_nibble >= 0xA && left_nibble <= 0xF && test_h(regs.byte.F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                regs.byte.A += 0x60;
                SET_C(regs.byte.F);
            } else if (left_nibble >= 0x9 && left_nibble <= 0xF && test_h(regs.byte.F) == 0 && right_nibble >= 0xA && right_nibble <= 0xF) {
                regs.byte.A += 0x66;
                SET_C(regs.byte.F);
            } else if (left_nibble >= 0xA && left_nibble <= 0xF && test_h(regs.byte.F) && right_nibble >= 0x0 && right_nibble <= 0x3) {
                regs.byte.A += 0x66;
                SET_C(regs.byte.F);
            }
        } else {
            if (left_nibble >= 0x0 && left_nibble <= 0x2 && test_h(regs.byte.F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                regs.byte.A += 0x60;
                SET_C(regs.byte.F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x2 && test_h(regs.byte.F) == 0 && right_nibble >= 0xA && right_nibble <= 0xF) {
                regs.byte.A += 0x66;
                SET_C(regs.byte.F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x3 && test_h(regs.byte.F) && right_nibble >= 0x0 && right_nibble <= 0x3) {
                regs.byte.A += 0x66;
                SET_C(regs.byte.F);
            }
        }
    } else {
        if (test_c(regs.byte.F) == 0) {
            if (left_nibble >= 0x0 && left_nibble <= 0x9 && test_h(regs.byte.F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                regs.byte.A += 0x00;
                RESET_C(regs.byte.F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x8 && test_h(regs.byte.F) && right_nibble >= 0x6 && right_nibble <= 0xF) {
                regs.byte.A += 0xFA;
                RESET_C(regs.byte.F);
            }
        } else {
            if (left_nibble >= 0x7 && left_nibble <= 0xF && test_h(regs.byte.F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                regs.byte.A += 0xA0;
                SET_C(regs.byte.F);
            } else if (left_nibble >= 0x6 && left_nibble <= 0xF && test_h(regs.byte.F) && right_nibble >= 0x6 && right_nibble <= 0xF) {
                regs.byte.A += 0x9A;
                SET_C(regs.byte.F);
            }
        }
    }
}

// JR Z, r8
void opcode_28()
{
    if (test_z(regs.byte.F)) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// ADD HL, HL
void opcode_29()
{
    RESET_N(regs.byte.F);
    if ((((regs.word.HL & 0x0FFF) + (regs.word.HL & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.word.HL >= regs.word.HL) {
        if ((regs.word.HL + regs.word.HL) >= regs.word.HL) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.HL + regs.word.HL) < regs.word.HL) {
            SET_C(regs.byte.F);
        }
    } else {
        if ((regs.word.HL + regs.word.HL) >= regs.word.HL) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.HL + regs.word.HL) < regs.word.HL) {
            SET_C(regs.byte.F);
        }
    }
    regs.word.HL += regs.word.HL;
}

// LD A, (HL+)
void opcode_2A()
{
    regs.byte.A = memory[regs.word.HL];
    ++regs.word.HL;
}

// DEC HL
void opcode_2B()
{
    --regs.word.HL;
}

// INC L
void opcode_2C()
{
    if (((0x01 & regs.byte.L) == 0x01) && ((0x02 & regs.byte.L) == 0x02) && ((0x04 & regs.byte.L) == 0x04) && ((0x08 & regs.byte.L) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.L;
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC L
void opcode_2D()
{
    if (((0x01 & regs.byte.L) == 0x00) && ((0x02 & regs.byte.L) == 0x00) && ((0x04 & regs.byte.L) == 0x00) && ((0x08 & regs.byte.L) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.L;
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD L, d8
void opcode_2E()
{
    regs.byte.L = memory[pc.PC++];
}

// CPL
void opcode_2F()
{
    regs.byte.A = ~regs.byte.A;
    SET_N(regs.byte.F);
    SET_H(regs.byte.F);
}

// JR NC, r8
void opcode_30()
{
    if (test_c(regs.byte.F) == 0) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// LD SP, d16
void opcode_31()
{
    sp.byte.L = memory[pc.PC++];
    sp.byte.H = memory[pc.PC++];
}

// LD (HL-), A
void opcode_32()
{
    memory[regs.word.HL] = regs.byte.A;
    --regs.word.HL;
}

// INC SP
void opcode_33()
{
    ++sp.SP;
}

// INC (HL)
void opcode_34()
{
    if (((0x01 & memory[regs.word.HL]) == 0x01) && ((0x02 & memory[regs.word.HL]) == 0x02) && ((0x04 & memory[regs.word.HL]) == 0x04) && ((0x08 & memory[regs.word.HL]) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++memory[regs.word.HL];
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC (HL)
void opcode_35()
{
    if (((0x01 & memory[regs.word.HL]) == 0x00) && ((0x02 & memory[regs.word.HL]) == 0x00) && ((0x04 & memory[regs.word.HL]) == 0x00) && ((0x08 & memory[regs.word.HL]) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --memory[regs.word.HL];
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD (HL), d8
void opcode_36()
{
    memory[regs.word.HL] = memory[pc.PC++];
}

// SCF
void opcode_37()
{
    SET_C(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_N(regs.byte.F);
}

// JR C, r8
void opcode_38()
{
    if (test_c(regs.byte.F)) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// ADD HL, SP
void opcode_39()
{
    RESET_N(regs.byte.F);
    if ((((regs.word.HL & 0x0FFF) + (sp.SP & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.word.HL >= sp.SP) {
        if ((regs.word.HL + sp.SP) >= regs.word.HL) {
            RESET_C(regs.byte.F);
        } else if ((regs.word.HL + sp.SP) < regs.word.HL) {
            SET_C(regs.byte.F);
        }
    } else {
        if ((sp.SP + regs.word.HL) >= sp.SP) {
            RESET_C(regs.byte.F);
        } else if ((sp.SP + regs.word.HL) < sp.SP) {
            SET_C(regs.byte.F);
        }
    }
    regs.word.HL += sp.SP;
}

// LD A, (HL-)
void opcode_3A()
{
    regs.byte.A = memory[regs.word.HL];
    --regs.word.HL;
}

// DEC SP
void opcode_3B()
{
    --sp.SP;
}

// INC A
void opcode_3C()
{
    if (((0x01 & regs.byte.A) == 0x01) && ((0x02 & regs.byte.A) == 0x02) && ((0x04 & regs.byte.A) == 0x04) && ((0x08 & regs.byte.A) == 0x08)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    ++regs.byte.A;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    RESET_N(regs.byte.F); // reset N flag
}

// DEC A
void opcode_3D()
{
    if (((0x01 & regs.byte.A) == 0x00) && ((0x02 & regs.byte.A) == 0x00) && ((0x04 & regs.byte.A) == 0x00) && ((0x08 & regs.byte.A) == 0x00)) {
        SET_H(regs.byte.F); // set H flag
    } else {
        RESET_H(regs.byte.F); // reset H flag
    }
    --regs.byte.A;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F); // set Z flag
    } else {
        RESET_Z(regs.byte.F); // reset Z flag
    }
    SET_N(regs.byte.F); // set N flag
}

// LD A, d8
void opcode_3E()
{
    regs.byte.A = memory[pc.PC++];
}

// CCF
void opcode_3F()
{
    if ((regs.byte.F & 0x10) == 0) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    RESET_H(regs.byte.F);
    RESET_N(regs.byte.F);
}

// LD B, B
void opcode_40()
{
    regs.byte.B = regs.byte.B;
}

// LD B, C
void opcode_41()
{
    regs.byte.B = regs.byte.C;
}

// LD B, D
void opcode_42()
{
    regs.byte.B = regs.byte.D;
}

// LD B, E
void opcode_43()
{
    regs.byte.B = regs.byte.E;
}

// LD B, H
void opcode_44()
{
    regs.byte.B = regs.byte.H;
}

// LD B, L
void opcode_45()
{
    regs.byte.B = regs.byte.L;
}

// LD B, (HL)
void opcode_46()
{
    regs.byte.B = memory[regs.word.HL];
}

// LD B, A
void opcode_47()
{
    regs.byte.B = regs.byte.A;
}

// LD C, B
void opcode_48()
{
    regs.byte.C = regs.byte.B;
}

// LD C, C
void opcode_49()
{
    regs.byte.C = regs.byte.C;
}

// LD C, D
void opcode_4A()
{
    regs.byte.C = regs.byte.D;
}

// LD C, E
void opcode_4B()
{
    regs.byte.C = regs.byte.E;
}

// LD C, H
void opcode_4C()
{
    regs.byte.C = regs.byte.H;
}

// LD C, L
void opcode_4D()
{
    regs.byte.C = regs.byte.L;
}

// LD C, (HL)
void opcode_4E()
{
    regs.byte.C = memory[regs.word.HL];
}

// LD C, A
void opcode_4F()
{
    regs.byte.C = regs.byte.A;
}

// LD D, B
void opcode_50()
{
    regs.byte.D = regs.byte.B;
}

// LD D, C
void opcode_51()
{
    regs.byte.D = regs.byte.C;
}

// LD D, D
void opcode_52()
{
    regs.byte.D = regs.byte.D;
}

// LD D, E
void opcode_53()
{
    regs.byte.D = regs.byte.E;
}

// LD D, H
void opcode_54()
{
    regs.byte.D = regs.byte.H;
}

// LD D, L
void opcode_55()
{
    regs.byte.D = regs.byte.L;
}

// LD D, (HL)
void opcode_56()
{
    regs.byte.D = memory[regs.word.HL];
}

// LD D, A
void opcode_57()
{
    regs.byte.D = regs.byte.A;
}

// LD E, B
void opcode_58()
{
    regs.byte.E = regs.byte.B;
}

// LD E, C
void opcode_59()
{
    regs.byte.E = regs.byte.C;
}

// LD E, D
void opcode_5A()
{
    regs.byte.E = regs.byte.D;
}

// LD E, E
void opcode_5B()
{
    regs.byte.E = regs.byte.E;
}

// LD E, H
void opcode_5C()
{
    regs.byte.E = regs.byte.H;
}

// LD E, L
void opcode_5D()
{
    regs.byte.E = regs.byte.L;
}

// LD E, (HL)
void opcode_5E()
{
    regs.byte.E = memory[regs.word.HL];
}

// LD E, A
void opcode_5F()
{
    regs.byte.E = regs.byte.A;
}

// LD H, B
void opcode_60()
{
    regs.byte.H = regs.byte.B;
}

// LD H, C
void opcode_61()
{
    regs.byte.H = regs.byte.C;
}

// LD H, D
void opcode_62()
{
    regs.byte.H = regs.byte.D;
}

// LD H, E
void opcode_63()
{
    regs.byte.H = regs.byte.E;
}

// LD H, H
void opcode_64()
{
    regs.byte.H = regs.byte.H;
}

// LD H, L
void opcode_65()
{
    regs.byte.H = regs.byte.L;
}

// LD H, (HL)
void opcode_66()
{
    regs.byte.H = memory[regs.word.HL];
}

// LD H, A
void opcode_67()
{
    regs.byte.H = regs.byte.A;
}

// LD L, B
void opcode_68()
{
    regs.byte.L = regs.byte.B;
}

// LD L, C
void opcode_69()
{
    regs.byte.L = regs.byte.C;
}

// LD L, D
void opcode_6A()
{
    regs.byte.L = regs.byte.D;
}

// LD L, E
void opcode_6B()
{
    regs.byte.L = regs.byte.E;
}

// LD L, H
void opcode_6C()
{
    regs.byte.L = regs.byte.H;
}

// LD L, L
void opcode_6D()
{
    regs.byte.L = regs.byte.L;
}

// LD L, (HL)
void opcode_6E()
{
    regs.byte.L = memory[regs.word.HL];
}

// LD L, A
void opcode_6F()
{
    regs.byte.L = regs.byte.A;
}

// LD (HL), B
void opcode_70()
{
    memory[regs.word.HL] = regs.byte.B;
}

// LD (HL), C
void opcode_71()
{
    memory[regs.word.HL] = regs.byte.C;
}

// LD (HL), D
void opcode_72()
{
    memory[regs.word.HL] = regs.byte.D;
}

// LD (HL), E
void opcode_73()
{
    memory[regs.word.HL] = regs.byte.E;
}

// LD (HL), H
void opcode_74()
{
    memory[regs.word.HL] = regs.byte.H;
}

// LD (HL), L
void opcode_75()
{
    memory[regs.word.HL] = regs.byte.L;
}

// HALT TODO
void opcode_76()
{

}

// LD (HL), A
void opcode_77()
{
    memory[regs.word.HL] = regs.byte.A;
}

// LD A, B
void opcode_78()
{
    regs.byte.A = regs.byte.B;
}

// LD A, C
void opcode_79()
{
    regs.byte.A = regs.byte.C;
}

// LD A, D
void opcode_7A()
{
    regs.byte.A = regs.byte.D;
}

// LD A, E
void opcode_7B()
{
    regs.byte.A = regs.byte.E;
}

// LD A, H
void opcode_7C()
{
    regs.byte.A = regs.byte.H;
}

// LD A, L
void opcode_7D()
{
    regs.byte.A = regs.byte.L;
}

// LD A, (HL)
void opcode_7E()
{
    regs.byte.A = memory[regs.word.HL];
}

// LD A, A
void opcode_7F()
{
    regs.byte.A = regs.byte.A;
}

// ADD A, B
void opcode_80()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.B & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.B;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.B;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, C
void opcode_81()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.C & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.C;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.C;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, D
void opcode_82()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.D & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.D;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.D;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, E
void opcode_83()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.E & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.E;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.E;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, H
void opcode_84()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.H & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.H;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.H;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, L
void opcode_85()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.L & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.L;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.L;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, (HL)
void opcode_86()
{
    if ((((regs.byte.A & 0x0F) + (memory[regs.word.HL] & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = memory[regs.word.HL];
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += memory[regs.word.HL];
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADD A, A
void opcode_87()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.A & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.A;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += regs.byte.A;
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, B
void opcode_88()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.B & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.B;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.B + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, C
void opcode_89()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.C & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.C;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.C + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, D
void opcode_8A()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.D & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.D;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.D + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, E
void opcode_8B()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.E & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.E;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.E + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, H
void opcode_8C()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.H & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.H;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.H + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, L
void opcode_8D()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.L & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.L;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.L + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, (HL)
void opcode_8E()
{
    if ((((regs.byte.A & 0x0F) + (memory[regs.word.HL] & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = memory[regs.word.HL];
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += memory[regs.word.HL] + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// ADC A, A
void opcode_8F()
{
    if ((((regs.byte.A & 0x0F) + (regs.byte.A & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = regs.byte.A;
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += regs.byte.A + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// SUB B
void opcode_90()
{
    if ((regs.byte.A & 0x0F) < (regs.byte.B & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.B) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= regs.byte.B;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB C
void opcode_91()
{
    if ((regs.byte.A & 0x0F) < (regs.byte.C & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.C) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= regs.byte.C;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB D
void opcode_92()
{
    if ((regs.byte.A & 0x0F) < (regs.byte.D & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.D) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= regs.byte.D;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB E
void opcode_93()
{
    if ((regs.byte.A & 0x0F) < (regs.byte.E & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.E) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= regs.byte.E;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB H
void opcode_94()
{
    if ((regs.byte.A & 0x0F) < (regs.byte.H & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.H) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= regs.byte.H;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB L
void opcode_95()
{
    if ((regs.byte.A & 0x0F) < (regs.byte.L & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.L) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= regs.byte.L;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB (HL)
void opcode_96()
{
    if ((regs.byte.A & 0x0F) < (memory[regs.word.HL] & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < memory[regs.word.HL]) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= memory[regs.word.HL];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SUB A
void opcode_97()
{
    regs.byte.A -= regs.byte.A;
    SET_Z(regs.byte.F);
    SET_N(regs.byte.F);
}

// SBC A, B
void opcode_98()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.B & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.B;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.B + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, C
void opcode_99()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.C & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.C;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.C + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, D
void opcode_9A()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.D & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.D;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.D + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, E
void opcode_9B()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.E & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.E;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.E + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, H
void opcode_9C()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.H & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.H;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.H + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, L
void opcode_9D()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.L & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.L;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.L + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, (HL)
void opcode_9E()
{
    if ((regs.byte.A & 0x0F) < ((memory[regs.word.HL] & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = memory[regs.word.HL];
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= memory[regs.word.HL] + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// SBC A, A
void opcode_9F()
{
    if ((regs.byte.A & 0x0F) < ((regs.byte.A & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = regs.byte.A;
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= regs.byte.A + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// AND B
void opcode_A0()
{
    regs.byte.A = regs.byte.A & regs.byte.B;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND C
void opcode_A1()
{
    regs.byte.A = regs.byte.A & regs.byte.C;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND D
void opcode_A2()
{
    regs.byte.A = regs.byte.A & regs.byte.D;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND E
void opcode_A3()
{
    regs.byte.A = regs.byte.A & regs.byte.E;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND H
void opcode_A4()
{
    regs.byte.A = regs.byte.A & regs.byte.H;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND L
void opcode_A5()
{
    regs.byte.A = regs.byte.A & regs.byte.L;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND (HL)
void opcode_A6()
{
    regs.byte.A = regs.byte.A & memory[regs.word.HL];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// AND A
void opcode_A7()
{
    regs.byte.A = regs.byte.A & regs.byte.A;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR B
void opcode_A8()
{
    regs.byte.A = regs.byte.A ^ regs.byte.B;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR C
void opcode_A9()
{
    regs.byte.A = regs.byte.A ^ regs.byte.C;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR D
void opcode_AA()
{
    regs.byte.A = regs.byte.A ^ regs.byte.D;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR E
void opcode_AB()
{
    regs.byte.A = regs.byte.A ^ regs.byte.B;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR H
void opcode_AC()
{
    regs.byte.A = regs.byte.A ^ regs.byte.H;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR L
void opcode_AD()
{
    regs.byte.A = regs.byte.A ^ regs.byte.L;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR (HL)
void opcode_AE()
{
    regs.byte.A = regs.byte.A ^ memory[regs.word.HL];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// XOR A
void opcode_AF()
{
    regs.byte.A = regs.byte.A ^ regs.byte.A;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR B
void opcode_B0()
{
    regs.byte.A = regs.byte.A | regs.byte.B;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR C
void opcode_B1()
{
    regs.byte.A = regs.byte.A | regs.byte.C;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR D
void opcode_B2()
{
    regs.byte.A = regs.byte.A | regs.byte.D;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR E
void opcode_B3()
{
    regs.byte.A = regs.byte.A | regs.byte.E;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR H
void opcode_B4()
{
    regs.byte.A = regs.byte.A | regs.byte.H;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR L
void opcode_B5()
{
    regs.byte.A = regs.byte.A | regs.byte.L;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR (HL)
void opcode_B6()
{
    regs.byte.A = regs.byte.A | memory[regs.word.HL];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// OR A
void opcode_B7()
{
    regs.byte.A = regs.byte.A | regs.byte.A;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// CP B
void opcode_B8()
{
    if ((regs.byte.A - regs.byte.B) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.B) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (regs.byte.B & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP C
void opcode_B9()
{
    if ((regs.byte.A - regs.byte.C) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.C) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (regs.byte.C & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP D
void opcode_BA()
{
    if ((regs.byte.A - regs.byte.D) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.D) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (regs.byte.D & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP E
void opcode_BB()
{
    if ((regs.byte.A - regs.byte.E) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.E) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (regs.byte.E & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP H
void opcode_BC()
{
    if ((regs.byte.A - regs.byte.H) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.H) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (regs.byte.H & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP L
void opcode_BD()
{
    if ((regs.byte.A - regs.byte.L) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < regs.byte.L) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (regs.byte.L & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP (HL)
void opcode_BE()
{
    if ((regs.byte.A - memory[regs.word.HL]) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < memory[regs.word.HL]) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (memory[regs.word.HL] & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// CP A
void opcode_BF()
{
    SET_Z(regs.byte.F);
    RESET_C(regs.byte.F);
    RESET_H(regs.byte.F);
    SET_N(regs.byte.F);
}

// RET NZ
void opcode_C0()
{
    if (test_z(regs.byte.F) == 0) {
        pc.byte.L = memory[sp.SP++];
        pc.byte.H = memory[sp.SP++];
    } else {
        counter += 12;
    }
}

// POP BC
void opcode_C1()
{
    regs.byte.C = memory[sp.SP++];
    regs.byte.B = memory[sp.SP++];
}

// JP NZ, a16
void opcode_C2()
{
    if (test_z(regs.byte.F) == 0) {
        BYTE lsb = memory[pc.PC];
        BYTE msb = memory[pc.PC+1];
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 4;
    }
}

// JP a16
void opcode_C3()
{
    BYTE lsb = memory[pc.PC];
    BYTE msb = memory[pc.PC+1];
    pc.byte.L = lsb;
    pc.byte.H = msb;
}

// CALL NZ, a16
void opcode_C4()
{
    if (test_z(regs.byte.F) == 0) {
        BYTE lsb = memory[pc.PC++];
        BYTE msb = memory[pc.PC++];
        memory[sp.SP-1] = pc.byte.H;
        memory[sp.SP-2] = pc.byte.L;
        sp.SP = sp.SP - 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 12;
    }
}

// PUSH BC
void opcode_C5()
{
    memory[sp.SP-1] = regs.byte.B;
    memory[sp.SP-2] = regs.byte.C;
    sp.SP = sp.SP - 2;
}

// ADD A, d8
void opcode_C6()
{
    if ((((regs.byte.A & 0x0F) + (memory[pc.PC] & 0x0F)) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = memory[pc.PC];
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A += memory[pc.PC++];
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// RST 00H
void opcode_C7()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x00;
}

// RET Z
void opcode_C8()
{
    if (test_z(regs.byte.F)) {
        pc.byte.L = memory[sp.SP++];
        pc.byte.H = memory[sp.SP++];
    } else {
        counter += 12;
    }
}

// RET
void opcode_C9()
{
    pc.byte.L = memory[sp.SP++];
    pc.byte.H = memory[sp.SP++];
}

// JP Z, a16
void opcode_CA()
{
    if (test_z(regs.byte.F)) {
        BYTE lsb = memory[pc.PC];
        BYTE msb = memory[pc.PC+1];
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 4;
    }
}

// PREFIX
void opcode_CB()
{
    opcode = memory[pc.PC++];
    switch (opcode) {
        case 0x00: pre_opcode_00(); break;
    }
}

// CALL Z, a16
void opcode_CC()
{
    if (test_z(regs.byte.F)) {
        BYTE lsb = memory[pc.PC++];
        BYTE msb = memory[pc.PC++];
        memory[sp.SP-1] = pc.byte.H;
        memory[sp.SP-2] = pc.byte.L;
        sp.SP = sp.SP - 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 12;
    }
}

// CALL a16
void opcode_CD()
{
    BYTE lsb = memory[pc.PC++];
    BYTE msb = memory[pc.PC++];
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.byte.L = lsb;
    pc.byte.H = msb;
}

// ADC A, d8
void opcode_CE()
{
    if ((((regs.byte.A & 0x0F) + (memory[pc.PC] & 0x0F) + (test_c(regs.byte.F))) & 0x10) == 0x10) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    unsigned int temp_byte1 = regs.byte.A;
    unsigned int temp_byte2 = memory[pc.PC];
    unsigned int temp_byte3 = test_c(regs.byte.F);
    regs.byte.A += memory[pc.PC++] + test_c(regs.byte.F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0x00) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
}

// RST 08H
void opcode_CF()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x08;
}

// RET NC
void opcode_D0()
{
    if (test_c(regs.byte.F) == 0) {
        pc.byte.L = memory[sp.SP++];
        pc.byte.H = memory[sp.SP++];
    } else {
        counter += 12;
    }
}

// POP DE
void opcode_D1()
{
    regs.byte.E = memory[sp.SP++];
    regs.byte.D = memory[sp.SP++];
}

// JP NC, a16
void opcode_D2()
{
    if (test_c(regs.byte.F) == 0) {
        BYTE lsb = memory[pc.PC];
        BYTE msb = memory[pc.PC+1];
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 4;
    }
}

// CALL NC, a16
void opcode_D4()
{
    if (test_c(regs.byte.F) == 0) {
        BYTE lsb = memory[pc.PC++];
        BYTE msb = memory[pc.PC++];
        memory[sp.SP-1] = pc.byte.H;
        memory[sp.SP-2] = pc.byte.L;
        sp.SP = sp.SP - 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 12;
    }
}

// PUSH DE
void opcode_D5()
{
    memory[sp.SP-1] = regs.byte.D;
    memory[sp.SP-2] = regs.byte.E;
    sp.SP = sp.SP - 2;
}

// SUB d8
void opcode_D6()
{
    if ((regs.byte.A & 0x0F) < (memory[pc.PC] & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    if (regs.byte.A < memory[pc.PC]) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    regs.byte.A -= memory[pc.PC++];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// RST 10H
void opcode_D7()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x10;
}

// RET C
void opcode_D8()
{
    if (test_c(regs.byte.F)) {
        pc.byte.L = memory[sp.SP++];
        pc.byte.H = memory[sp.SP++];
    } else {
        counter += 12;
    }
}

// RETI
void opcode_D9()
{
    pc.byte.L = memory[sp.SP++];
    pc.byte.H = memory[sp.SP++];
    enable_interrupt = 1;
}

// JP C, a16
void opcode_DA()
{
    if (test_c(regs.byte.F)) {
        BYTE lsb = memory[pc.PC];
        BYTE msb = memory[pc.PC+1];
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 4;
    }
}

// CALL C, a16
void opcode_DC()
{
    if (test_c(regs.byte.F)) {
        BYTE lsb = memory[pc.PC++];
        BYTE msb = memory[pc.PC++];
        memory[sp.SP-1] = pc.byte.H;
        memory[sp.SP-2] = pc.byte.L;
        sp.SP = sp.SP - 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        counter += 12;
    }
}

// SBC A, d8
void opcode_DE()
{
    if ((regs.byte.A & 0x0F) < ((memory[pc.PC] & 0x0F) + test_c(regs.byte.F))) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    int temp1 = regs.byte.A;
    int temp2 = memory[pc.PC];
    int temp3 = test_c(regs.byte.F);
    regs.byte.A -= memory[pc.PC++] + test_c(regs.byte.F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// RST 18H
void opcode_DF()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x18;
}

// LDH (a8), A
void opcode_E0()
{
    BYTE temp = memory[pc.PC++];
    memory[0xFF00+temp] = regs.byte.A;
}

// POP HL
void opcode_E1()
{
    regs.byte.L = memory[sp.SP++];
    regs.byte.H = memory[sp.SP++];
}

// LD (C), A
void opcode_E2()
{
    memory[0xFF00+regs.byte.C] = regs.byte.A;
}

// PUSH HL
void opcode_E5()
{
    memory[sp.SP-1] = regs.byte.H;
    memory[sp.SP-2] = regs.byte.L;
    sp.SP = sp.SP - 2;
}

// AND d8
void opcode_E6()
{
    regs.byte.A = regs.byte.A & memory[pc.PC++];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    SET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// RST 20H
void opcode_E7()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x20;
}

// ADD SP, r8 TODO
void opcode_E8()
{

}

// JP (HL)
void opcode_E9()
{
    pc.PC = regs.word.HL;
}

// LD (a16), A
void opcode_EA()
{
    WORD lsb = memory[pc.PC++];
    WORD msb = memory[pc.PC++];
    msb = msb << 8;
    WORD temp = msb + lsb;
    memory[temp] = regs.byte.A;
}

// XOR d8
void opcode_EE()
{
    regs.byte.A = regs.byte.A ^ memory[pc.PC++];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// RST 28H
void opcode_EF()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x28;
}

// LDH A, (a8)
void opcode_F0()
{
    BYTE temp = memory[pc.PC++];
    regs.byte.A = memory[0xFF00+temp];
}

// POP AF
void opcode_F1()
{
    regs.byte.F = memory[sp.SP];
    regs.byte.A = memory[sp.SP+1];
    sp.SP = sp.SP + 2;
}

// LD A, (C)
void opcode_F2()
{
    regs.byte.A = memory[0xFF00+regs.byte.C];
}

// DI
void opcode_F3()
{
    enable_interrupt = 0;
}

// PUSH AF
void opcode_F5()
{
    memory[sp.SP-1] = regs.byte.A;
    memory[sp.SP-2] = regs.byte.F;
    sp.SP = sp.SP - 2;
}

// OR d8
void opcode_F6()
{
    regs.byte.A = regs.byte.A | memory[pc.PC++];
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// RST 30H
void opcode_F7()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x30;
}

// LD HL, SP+r8 TODO
void opcode_F8()
{

}

// LD SP, HL
void opcode_F9()
{
    sp.SP = regs.word.HL;
}

// LD A, (a16)
void opcode_FA()
{
    WORD lsb = memory[pc.PC++];
    WORD msb = memory[pc.PC++];
    msb = msb << 8;
    WORD temp = msb + lsb;
    regs.byte.A = memory[temp];
}

// EI
void opcode_FB()
{
    enable_interrupt = 1;
}

// CP d8
void opcode_FE()
{
    if ((regs.byte.A - memory[pc.PC]) == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    if (regs.byte.A < memory[pc.PC]) {
        SET_C(regs.byte.F);
    } else {
        RESET_C(regs.byte.F);
    }
    if ((regs.byte.A & 0x0F) < (memory[pc.PC++] & 0x0F)) {
        SET_H(regs.byte.F);
    } else {
        RESET_H(regs.byte.F);
    }
    SET_N(regs.byte.F);
}

// RST 38H
void opcode_FF()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x38;
}
