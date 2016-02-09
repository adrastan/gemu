#include "pre_opcodes.h"
#include "cpu.h"
#include "memory.h"
#include "registers.h"

// RLC B
void pre_opcode_00()
{
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.B >> 7) & 1; // get 7th bit
    regs.byte.B = regs.byte.B << 1; // shift left 1
    regs.byte.B ^= (-bit ^ regs.byte.B) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC C
void pre_opcode_01()
{
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.C >> 7) & 1; // get 7th bit
    regs.byte.C = regs.byte.C << 1; // shift left 1
    regs.byte.C ^= (-bit ^ regs.byte.C) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC D
void pre_opcode_02()
{
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.D >> 7) & 1; // get 7th bit
    regs.byte.D = regs.byte.D << 1; // shift left 1
    regs.byte.D ^= (-bit ^ regs.byte.D) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC E
void pre_opcode_03()
{
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.E >> 7) & 1; // get 7th bit
    regs.byte.E = regs.byte.E << 1; // shift left 1
    regs.byte.E ^= (-bit ^ regs.byte.E) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC H
void pre_opcode_04()
{
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.H >> 7) & 1; // get 7th bit
    regs.byte.H = regs.byte.H << 1; // shift left 1
    regs.byte.H ^= (-bit ^ regs.byte.H) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC L
void pre_opcode_05()
{
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.L >> 7) & 1; // get 7th bit
    regs.byte.L = regs.byte.L << 1; // shift left 1
    regs.byte.L ^= (-bit ^ regs.byte.L) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC (HL)
void pre_opcode_06()
{
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (memory[regs.word.HL] >> 7) & 1; // get 7th bit
    memory[regs.word.HL] = memory[regs.word.HL] << 1; // shift left 1
    memory[regs.word.HL] ^= (-bit ^ memory[regs.word.HL]) & (1 << 0x00); // copy bit 7 to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RLC A
void pre_opcode_07()
{
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
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

// RRC B
void pre_opcode_08()
{
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.B >> 0) & 1; // get 0th bit
    regs.byte.B = regs.byte.B >> 1; // shift right 1
    regs.byte.B ^= (-bit ^ regs.byte.B) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC C
void pre_opcode_09()
{
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.C >> 0) & 1; // get 0th bit
    regs.byte.C = regs.byte.C >> 1; // shift right 1
    regs.byte.C ^= (-bit ^ regs.byte.C) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC D
void pre_opcode_0A()
{
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.D >> 0) & 1; // get 0th bit
    regs.byte.D = regs.byte.D >> 1; // shift right 1
    regs.byte.D ^= (-bit ^ regs.byte.D) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC E
void pre_opcode_0B()
{
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.E >> 0) & 1; // get 0th bit
    regs.byte.E = regs.byte.E >> 1; // shift right 1
    regs.byte.E ^= (-bit ^ regs.byte.E) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC H
void pre_opcode_0C()
{
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.H >> 0) & 1; // get 0th bit
    regs.byte.H = regs.byte.H >> 1; // shift right 1
    regs.byte.H ^= (-bit ^ regs.byte.H) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC L
void pre_opcode_0D()
{
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.L >> 0) & 1; // get 0th bit
    regs.byte.L = regs.byte.L >> 1; // shift right 1
    regs.byte.L ^= (-bit ^ regs.byte.L) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC (HL)
void pre_opcode_0E()
{
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
        return;
    }
    BYTE bit = (memory[regs.word.HL] >> 0) & 1; // get 0th bit
    memory[regs.word.HL] = memory[regs.word.HL] >> 1; // shift right 1
    memory[regs.word.HL ]^= (-bit ^ memory[regs.word.HL]) & (1 << 0x07); // copy bit 0 to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RRC A
void pre_opcode_0F()
{
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        RESET_C(regs.byte.F);
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

// RL B
void pre_opcode_10()
{
    if (regs.byte.B == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.B = regs.byte.B << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.B == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.B >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.B = regs.byte.B << 1; // shift left 1
    regs.byte.B ^= (-carry ^ regs.byte.B) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL C
void pre_opcode_11()
{
    if (regs.byte.C == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.C = regs.byte.C << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.C == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.C >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.C = regs.byte.C << 1; // shift left 1
    regs.byte.C ^= (-carry ^ regs.byte.C) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL D
void pre_opcode_12()
{
    if (regs.byte.D == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.D = regs.byte.D << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.D == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.D >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.D = regs.byte.D << 1; // shift left 1
    regs.byte.D ^= (-carry ^ regs.byte.D) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL E
void pre_opcode_13()
{
    if (regs.byte.E == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.E = regs.byte.E << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.E == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.E >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.E = regs.byte.E << 1; // shift left 1
    regs.byte.E ^= (-carry ^ regs.byte.E) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL H
void pre_opcode_14()
{
    if (regs.byte.H == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.H = regs.byte.H << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.H == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.H >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.H = regs.byte.H << 1; // shift left 1
    regs.byte.H ^= (-carry ^ regs.byte.H) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL L
void pre_opcode_15()
{
    if (regs.byte.L == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.L = regs.byte.L << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.L == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.L >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.L = regs.byte.L << 1; // shift left 1
    regs.byte.L ^= (-carry ^ regs.byte.L) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL (HL)
void pre_opcode_16()
{
    if (memory[regs.word.HL] == 0x80 && test_c(regs.byte.F) == 0) {
        memory[regs.word.HL] = memory[regs.word.HL] << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (memory[regs.word.HL] == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (memory[regs.word.HL] >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    memory[regs.word.HL] = memory[regs.word.HL] << 1; // shift left 1
    memory[regs.word.HL] ^= (-carry ^ memory[regs.word.HL]) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RL A
void pre_opcode_17()
{
    if (regs.byte.A == 0x80 && test_c(regs.byte.F) == 0) {
        regs.byte.A = regs.byte.A << 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.A == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.A >> 7) & 1; // get 7th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.A = regs.byte.A << 1; // shift left 1
    regs.byte.A ^= (-carry ^ regs.byte.A) & (1 << 0x00); // copy carry bit to bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR B
void pre_opcode_18()
{
    if (regs.byte.B == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.B = regs.byte.B >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.B == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.B >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.B = regs.byte.B >> 1; // shift right 1
    regs.byte.B ^= (-carry ^ regs.byte.B) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR C
void pre_opcode_19()
{
    if (regs.byte.C == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.C = regs.byte.C >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.C == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.C >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.C = regs.byte.C >> 1; // shift right 1
    regs.byte.C ^= (-carry ^ regs.byte.C) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR D
void pre_opcode_1A()
{
    if (regs.byte.D == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.D = regs.byte.D >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.D == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.D >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.D = regs.byte.D >> 1; // shift right 1
    regs.byte.D ^= (-carry ^ regs.byte.D) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR E
void pre_opcode_1B()
{
    if (regs.byte.E == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.E = regs.byte.E >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.E == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.E >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.E = regs.byte.E >> 1; // shift right 1
    regs.byte.E ^= (-carry ^ regs.byte.E) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR H
void pre_opcode_1C()
{
    if (regs.byte.H == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.H = regs.byte.H >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.H == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.H >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.H = regs.byte.H >> 1; // shift right 1
    regs.byte.H ^= (-carry ^ regs.byte.H) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR L
void pre_opcode_1D()
{
    if (regs.byte.L == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.L = regs.byte.L >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.L == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.L >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.L = regs.byte.L >> 1; // shift right 1
    regs.byte.L ^= (-carry ^ regs.byte.L) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR (HL)
void pre_opcode_1E()
{
    if (memory[regs.word.HL] == 0x01 && test_c(regs.byte.F) == 0) {
        memory[regs.word.HL] = memory[regs.word.HL] >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (memory[regs.word.HL] == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (memory[regs.word.HL] >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    memory[regs.word.HL] = memory[regs.word.HL] >> 1; // shift right 1
    memory[regs.word.HL] ^= (-carry ^ memory[regs.word.HL]) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// RR A
void pre_opcode_1F()
{
    if (regs.byte.A == 0x01 && test_c(regs.byte.F) == 0) {
        regs.byte.A = regs.byte.A >> 1;
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        SET_C(regs.byte.F);
        return;
    } else if (regs.byte.A == 0 && test_c(regs.byte.F) == 0) {
        SET_Z(regs.byte.F);
        RESET_N(regs.byte.F);
        RESET_H(regs.byte.F);
        return;
    }
    BYTE bit = (regs.byte.A >> 0) & 1; // get 0th bit
    BYTE carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.A = regs.byte.A >> 1; // shift right 1
    regs.byte.A ^= (-carry ^ regs.byte.A) & (1 << 0x07); // copy carry bit to bit 7
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(regs.byte.F); // reset Z flag
    RESET_N(regs.byte.F); // reset N flag
    RESET_H(regs.byte.F); // reset H flag
}

// SLA B
void pre_opcode_20()
{
    BYTE bit = (regs.byte.B >> 7) & 1; // get 7th bit
    regs.byte.B = regs.byte.B << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA C
void pre_opcode_21()
{
    BYTE bit = (regs.byte.C >> 7) & 1; // get 7th bit
    regs.byte.C = regs.byte.C << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA D
void pre_opcode_22()
{
    BYTE bit = (regs.byte.D >> 7) & 1; // get 7th bit
    regs.byte.D = regs.byte.D << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA E
void pre_opcode_23()
{
    BYTE bit = (regs.byte.E >> 7) & 1; // get 7th bit
    regs.byte.E = regs.byte.E << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA H
void pre_opcode_24()
{
    BYTE bit = (regs.byte.H >> 7) & 1; // get 7th bit
    regs.byte.H = regs.byte.H << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA L
void pre_opcode_25()
{
    BYTE bit = (regs.byte.L >> 7) & 1; // get 7th bit
    regs.byte.L = regs.byte.L << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA (HL)
void pre_opcode_26()
{
    BYTE bit = (memory[regs.word.HL] >> 7) & 1; // get 7th bit
    memory[regs.word.HL] = memory[regs.word.HL] << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SLA A
void pre_opcode_27()
{
    BYTE bit = (regs.byte.A >> 7) & 1; // get 7th bit
    regs.byte.A = regs.byte.A << 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
}

// SRA B
void pre_opcode_28()
{
    BYTE bit = (regs.byte.B >> 7) & 1; // get 7th bit
    regs.byte.B = regs.byte.B >> 1;
    regs.byte.B ^= (-bit ^ regs.byte.B) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA C
void pre_opcode_29()
{
    BYTE bit = (regs.byte.C >> 7) & 1; // get 7th bit
    regs.byte.C = regs.byte.C >> 1;
    regs.byte.C ^= (-bit ^ regs.byte.C) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA D
void pre_opcode_2A()
{
    BYTE bit = (regs.byte.D >> 7) & 1; // get 7th bit
    regs.byte.D = regs.byte.D >> 1;
    regs.byte.D ^= (-bit ^ regs.byte.D) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA E
void pre_opcode_2B()
{
    BYTE bit = (regs.byte.E >> 7) & 1; // get 7th bit
    regs.byte.E = regs.byte.E >> 1;
    regs.byte.E ^= (-bit ^ regs.byte.E) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA H
void pre_opcode_2C()
{
    BYTE bit = (regs.byte.H >> 7) & 1; // get 7th bit
    regs.byte.H = regs.byte.H >> 1;
    regs.byte.H ^= (-bit ^ regs.byte.H) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA L
void pre_opcode_2D()
{
    BYTE bit = (regs.byte.L >> 7) & 1; // get 7th bit
    regs.byte.L = regs.byte.L >> 1;
    regs.byte.L ^= (-bit ^ regs.byte.L) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA (HL)
void pre_opcode_2E()
{
    BYTE bit = (memory[regs.word.HL] >> 7) & 1; // get 7th bit
    memory[regs.word.HL] = memory[regs.word.HL] >> 1;
    memory[regs.word.HL] ^= (-bit ^ memory[regs.word.HL]) & (1 << 0x07); // copy bit 7 back
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SRA A
void pre_opcode_2F()
{
    BYTE bit = (regs.byte.A >> 7) & 1; // get 7th bit
    regs.byte.A = regs.byte.A >> 1;
    regs.byte.A ^= (-bit ^ regs.byte.A) & (1 << 0x07); // copy bit 7 back
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_C(regs.byte.F);
}

// SWAP B
void pre_opcode_30()
{
    BYTE low_nibble = 0x0F & regs.byte.B;
    BYTE high_nibble = 0xF0 & regs.byte.B;
    regs.byte.B = high_nibble + low_nibble;
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP C
void pre_opcode_31()
{
    BYTE low_nibble = 0x0F & regs.byte.C;
    BYTE high_nibble = 0xF0 & regs.byte.C;
    regs.byte.C = high_nibble + low_nibble;
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP D
void pre_opcode_32()
{
    BYTE low_nibble = 0x0F & regs.byte.D;
    BYTE high_nibble = 0xF0 & regs.byte.D;
    regs.byte.D = high_nibble + low_nibble;
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP E
void pre_opcode_33()
{
    BYTE low_nibble = 0x0F & regs.byte.E;
    BYTE high_nibble = 0xF0 & regs.byte.E;
    regs.byte.E = high_nibble + low_nibble;
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP H
void pre_opcode_34()
{
    BYTE low_nibble = 0x0F & regs.byte.H;
    BYTE high_nibble = 0xF0 & regs.byte.H;
    regs.byte.H = high_nibble + low_nibble;
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP L
void pre_opcode_35()
{
    BYTE low_nibble = 0x0F & regs.byte.L;
    BYTE high_nibble = 0xF0 & regs.byte.L;
    regs.byte.L = high_nibble + low_nibble;
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP (HL)
void pre_opcode_36()
{
    BYTE low_nibble = 0x0F & memory[regs.word.HL];
    BYTE high_nibble = 0xF0 & memory[regs.word.HL];
    memory[regs.word.HL] = high_nibble + low_nibble;
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SWAP A
void pre_opcode_37()
{
    BYTE low_nibble = 0x0F & regs.byte.A;
    BYTE high_nibble = 0xF0 & regs.byte.A;
    regs.byte.A = high_nibble + low_nibble;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
    RESET_C(regs.byte.F);
}

// SRL B
void pre_opcode_38()
{
    BYTE bit = 0x01 & regs.byte.B; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.B = regs.byte.B >> 1;
    if (regs.byte.B == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL C
void pre_opcode_39()
{
    BYTE bit = 0x01 & regs.byte.C; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.C = regs.byte.C >> 1;
    if (regs.byte.C == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL D
void pre_opcode_3A()
{
    BYTE bit = 0x01 & regs.byte.D; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.D = regs.byte.D >> 1;
    if (regs.byte.D == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL E
void pre_opcode_3B()
{
    BYTE bit = 0x01 & regs.byte.E; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.E = regs.byte.E >> 1;
    if (regs.byte.E == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL H
void pre_opcode_3C()
{
    BYTE bit = 0x01 & regs.byte.H; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.H = regs.byte.H >> 1;
    if (regs.byte.H == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL L
void pre_opcode_3D()
{
    BYTE bit = 0x01 & regs.byte.L; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.L = regs.byte.L >> 1;
    if (regs.byte.L == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL (HL)
void pre_opcode_3E()
{
    BYTE bit = 0x01 & memory[regs.word.HL]; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    memory[regs.word.HL] = memory[regs.word.HL] >> 1;
    if (memory[regs.word.HL] == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// SRL A
void pre_opcode_3F()
{
    BYTE bit = 0x01 & regs.byte.A; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    regs.byte.A = regs.byte.A >> 1;
    if (regs.byte.A == 0) {
        SET_Z(regs.byte.F);
    } else {
        RESET_Z(regs.byte.F);
    }
    RESET_N(regs.byte.F);
    RESET_H(regs.byte.F);
}

// BIT 0,B
void pre_opcode_40()
{

}
