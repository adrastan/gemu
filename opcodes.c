#include "cpu.h"
#include "opcodes.h"
#include "pre_opcodes.h"
#include "memory.h"
#include "registers.h"

extern int counter;
extern int enable_interrupt;
extern BYTE opcode;
extern int pre_cycles[];

// LD BC, d16
void opcode_01()
{
    *C = memory[pc.PC++];
    *B = memory[pc.PC++];
}

// LD (BC), A
void opcode_02()
{
    memory[*BC] = *A;
}

// INC BC
void opcode_03()
{
    ++*BC;
}

// INC B
void opcode_04()
{
    if (((0x01 & *B) == 0x01) && ((0x02 & *B) == 0x02) && ((0x04 & *B) == 0x04) && ((0x08 & *B) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*B;
    if (*B == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC B
void opcode_05()
{
    if (((0x01 & *B) == 0x00) && ((0x02 & *B) == 0x00) && ((0x04 & *B) == 0x00) && ((0x08 & *B) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*B;
    if (*B == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD B, d8
void opcode_06()
{
    *B = memory[pc.PC++];
}

// RLCA
void opcode_07()
{
    BYTE bit = (*A >> 7) & 1; // get 7th bit
    *A = *A << 1; // shift left 1
    *A ^= (-bit ^ *A) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// LD (a16), SP
void opcode_08()
{
    WORD lower = memory[pc.PC++];
    WORD higher = memory[pc.PC++];
    WORD address = (higher << 8) | lower;
    memory[address++] = sp.byte.L;
    memory[address] = sp.byte.H;
}

// ADD HL, BC
void opcode_09()
{
    RESET_N(*F);
    if ((((*HL & 0x0FFF) + (*BC & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*HL >= *BC) {
        if (((WORD)(*HL + *BC)) >= *HL) {
            RESET_C(*F);
        } else if (((WORD)(*HL + *BC)) < *HL) {
            SET_C(*F);
        }
    } else {
        if (((WORD)(*BC + *HL)) >= *BC) {
            RESET_C(*F);
        } else if (((WORD)(*BC + *HL)) < *BC) {
            SET_C(*F);
        }
    }
    *HL += *BC;
}

// LD A, (BC)
void opcode_0A()
{
    *A = memory[*BC];
}

// DEC BC
void opcode_0B()
{
    --*BC;
}

// INC C
void opcode_0C()
{
    if (((0x01 & *C) == 0x01) && ((0x02 & *C) == 0x02) && ((0x04 & *C) == 0x04) && ((0x08 & *C) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*C;
    if (*C == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC C
void opcode_0D()
{
    if (((0x01 & *C) == 0x00) && ((0x02 & *C) == 0x00) && ((0x04 & *C) == 0x00) && ((0x08 & *C) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*B;
    if (*B == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD C, d8
void opcode_0E()
{
    *C = memory[pc.PC++];
}

// RRCA
void opcode_0F()
{
    BYTE bit = (*A >> 0) & 1; // get 0th bit
    *A = *A >> 1; // shift right 1
    *A ^= (-bit ^ *A) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// STOP 0
void opcode_10()
{
    return;
}

// LD DE, d16
void opcode_11()
{
    *E = memory[pc.PC++];
    *D = memory[pc.PC++];
}

// LD (DE), A
void opcode_12()
{
    memory[*DE] = *A;
}

// INC DE
void opcode_13()
{
    ++*DE;
}

// INC D
void opcode_14()
{
    if (((0x01 & *D) == 0x01) && ((0x02 & *D) == 0x02) && ((0x04 & *D) == 0x04) && ((0x08 & *D) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*D;
    if (*D == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC D
void opcode_15()
{
    if (((0x01 & *D) == 0x00) && ((0x02 & *D) == 0x00) && ((0x04 & *D) == 0x00) && ((0x08 & *D) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*D;
    if (*D == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD D, d8
void opcode_16()
{
    *D = memory[pc.PC++];
}

// RLA
void opcode_17()
{
    BYTE bit = (*A >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *A = *A << 1; // shift left 1
    *A ^= (-carry ^ *A) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
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
    RESET_N(*F);
    if ((((*HL & 0x0FFF) + (*DE & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*HL >= *DE) {
        if ((*HL + *DE) >= *HL) {
            RESET_C(*F);
        } else if ((*HL + *DE) < *HL) {
            SET_C(*F);
        }
    } else {
        if ((*DE + *HL) >= *DE) {
            RESET_C(*F);
        } else if ((*DE + *HL) < *DE) {
            SET_C(*F);
        }
    }
    *HL += *DE;
}

// LD A, (DE)
void opcode_1A()
{
    *A = memory[*DE];
}

// DEC DE
void opcode_1B()
{
    --*DE;
}

// INC E
void opcode_1C()
{
    if (((0x01 & *E) == 0x01) && ((0x02 & *E) == 0x02) && ((0x04 & *E) == 0x04) && ((0x08 & *E) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*E;
    if (*E == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC E
void opcode_1D()
{
    if (((0x01 & *E) == 0x00) && ((0x02 & *E) == 0x00) && ((0x04 & *E) == 0x00) && ((0x08 & *E) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*E;
    if (*E == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD E, d8
void opcode_1E()
{
    *E = memory[pc.PC++];
}

// RRA
void opcode_1F()
{
    BYTE bit = (*A >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *A = *A >> 1; // shift left 1
    *A ^= (-carry ^ *A) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// JR NZ, r8
void opcode_20()
{
    if (test_z(*F) == 0) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// LD HL, d16
void opcode_21()
{
    *L = memory[pc.PC++];
    *H = memory[pc.PC++];
}

// LD (HL+), A
void opcode_22()
{
    memory[*HL] = *A;
    ++*HL;
}

// INC HL
void opcode_23()
{
    ++*HL;
}

// INC H
void opcode_24()
{
    if (((0x01 & *H) == 0x01) && ((0x02 & *H) == 0x02) && ((0x04 & *H) == 0x04) && ((0x08 & *H) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*H;
    if (*H == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC H
void opcode_25()
{
    if (((0x01 & *H) == 0x00) && ((0x02 & *H) == 0x00) && ((0x04 & *H) == 0x00) && ((0x08 & *H) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*H;
    if (*H == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD H, d8
void opcode_26()
{
    *H = memory[pc.PC++];
}

// DAA
void opcode_27()
{
    BYTE left_nibble = 0xF0 & *A;
    left_nibble = left_nibble >> 4;
    BYTE right_nibble = 0x0F & *A;

    if (test_n(*F) == 0) {
        if (test_c(*F) == 0) {
            if (left_nibble >= 0x0 && left_nibble <= 0x9 && test_h(*F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                *A += 0x00;
                RESET_C(*F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x8 && test_h(*F) == 0 && right_nibble >= 0xA && right_nibble <= 0xF) {
                *A += 0x06;
                RESET_C(*F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x9 && test_h(*F) && right_nibble >= 0x0 && right_nibble <= 0x3) {
                *A += 0x06;
                RESET_C(*F);
            } else if (left_nibble >= 0xA && left_nibble <= 0xF && test_h(*F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                *A += 0x60;
                SET_C(*F);
            } else if (left_nibble >= 0x9 && left_nibble <= 0xF && test_h(*F) == 0 && right_nibble >= 0xA && right_nibble <= 0xF) {
                *A += 0x66;
                SET_C(*F);
            } else if (left_nibble >= 0xA && left_nibble <= 0xF && test_h(*F) && right_nibble >= 0x0 && right_nibble <= 0x3) {
                *A += 0x66;
                SET_C(*F);
            }
        } else {
            if (left_nibble >= 0x0 && left_nibble <= 0x2 && test_h(*F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                *A += 0x60;
                SET_C(*F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x2 && test_h(*F) == 0 && right_nibble >= 0xA && right_nibble <= 0xF) {
                *A += 0x66;
                SET_C(*F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x3 && test_h(*F) && right_nibble >= 0x0 && right_nibble <= 0x3) {
                *A += 0x66;
                SET_C(*F);
            }
        }
    } else {
        if (test_c(*F) == 0) {
            if (left_nibble >= 0x0 && left_nibble <= 0x9 && test_h(*F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                *A += 0x00;
                RESET_C(*F);
            } else if (left_nibble >= 0x0 && left_nibble <= 0x8 && test_h(*F) && right_nibble >= 0x6 && right_nibble <= 0xF) {
                *A += 0xFA;
                RESET_C(*F);
            }
        } else {
            if (left_nibble >= 0x7 && left_nibble <= 0xF && test_h(*F) == 0 && right_nibble >= 0x0 && right_nibble <= 0x9) {
                *A += 0xA0;
                SET_C(*F);
            } else if (left_nibble >= 0x6 && left_nibble <= 0xF && test_h(*F) && right_nibble >= 0x6 && right_nibble <= 0xF) {
                *A += 0x9A;
                SET_C(*F);
            }
        }
    }
}

// JR Z, r8
void opcode_28()
{
    if (test_z(*F)) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// ADD HL, HL
void opcode_29()
{
    RESET_N(*F);
    if ((((*HL & 0x0FFF) + (*HL & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*HL >= *HL) {
        if ((*HL + *HL) >= *HL) {
            RESET_C(*F);
        } else if ((*HL + *HL) < *HL) {
            SET_C(*F);
        }
    } else {
        if ((*HL + *HL) >= *HL) {
            RESET_C(*F);
        } else if ((*HL + *HL) < *HL) {
            SET_C(*F);
        }
    }
    *HL += *HL;
}

// LD A, (HL+)
void opcode_2A()
{
    *A = memory[*HL];
    ++*HL;
}

// DEC HL
void opcode_2B()
{
    --*HL;
}

// INC L
void opcode_2C()
{
    if (((0x01 & *L) == 0x01) && ((0x02 & *L) == 0x02) && ((0x04 & *L) == 0x04) && ((0x08 & *L) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*L;
    if (*L == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC L
void opcode_2D()
{
    if (((0x01 & *L) == 0x00) && ((0x02 & *L) == 0x00) && ((0x04 & *L) == 0x00) && ((0x08 & *L) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*L;
    if (*L == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD L, d8
void opcode_2E()
{
    *L = memory[pc.PC++];
}

// CPL
void opcode_2F()
{
    *A = ~*A;
    SET_N(*F);
    SET_H(*F);
}

// JR NC, r8
void opcode_30()
{
    if (test_c(*F) == 0) {
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
    memory[*HL] = *A;
    --*HL;
}

// INC SP
void opcode_33()
{
    ++sp.SP;
}

// INC (HL)
void opcode_34()
{
    if (((0x01 & memory[*HL]) == 0x01) && ((0x02 & memory[*HL]) == 0x02) && ((0x04 & memory[*HL]) == 0x04) && ((0x08 & memory[*HL]) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++memory[*HL];
    if (memory[*HL] == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC (HL)
void opcode_35()
{
    if (((0x01 & memory[*HL]) == 0x00) && ((0x02 & memory[*HL]) == 0x00) && ((0x04 & memory[*HL]) == 0x00) && ((0x08 & memory[*HL]) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --memory[*HL];
    if (memory[*HL] == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD (HL), d8
void opcode_36()
{
    memory[*HL] = memory[pc.PC++];
}

// SCF
void opcode_37()
{
    SET_C(*F);
    RESET_H(*F);
    RESET_N(*F);
}

// JR C, r8
void opcode_38()
{
    if (test_c(*F)) {
        signed char byte = memory[pc.PC++];
        pc.PC += byte;
    } else {
        counter += 4;
    }
}

// ADD HL, SP
void opcode_39()
{
    RESET_N(*F);
    if ((((*HL & 0x0FFF) + (sp.SP & 0x0FFF)) & 0x1000) == 0x1000) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*HL >= sp.SP) {
        if ((*HL + sp.SP) >= *HL) {
            RESET_C(*F);
        } else if ((*HL + sp.SP) < *HL) {
            SET_C(*F);
        }
    } else {
        if ((sp.SP + *HL) >= sp.SP) {
            RESET_C(*F);
        } else if ((sp.SP + *HL) < sp.SP) {
            SET_C(*F);
        }
    }
    *HL += sp.SP;
}

// LD A, (HL-)
void opcode_3A()
{
    *A = memory[*HL];
    --*HL;
}

// DEC SP
void opcode_3B()
{
    --sp.SP;
}

// INC A
void opcode_3C()
{
    if (((0x01 & *A) == 0x01) && ((0x02 & *A) == 0x02) && ((0x04 & *A) == 0x04) && ((0x08 & *A) == 0x08)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    ++*A;
    if (*A == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    RESET_N(*F); // reset N flag
}

// DEC A
void opcode_3D()
{
    if (((0x01 & *A) == 0x00) && ((0x02 & *A) == 0x00) && ((0x04 & *A) == 0x00) && ((0x08 & *A) == 0x00)) {
        SET_H(*F); // set H flag
    } else {
        RESET_H(*F); // reset H flag
    }
    --*A;
    if (*A == 0) {
        SET_Z(*F); // set Z flag
    } else {
        RESET_Z(*F); // reset Z flag
    }
    SET_N(*F); // set N flag
}

// LD A, d8
void opcode_3E()
{
    *A = memory[pc.PC++];
}

// CCF
void opcode_3F()
{
    if ((*F & 0x10) == 0) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    RESET_H(*F);
    RESET_N(*F);
}

// LD B, B
void opcode_40()
{
    *B = *B;
}

// LD B, C
void opcode_41()
{
    *B = *C;
}

// LD B, D
void opcode_42()
{
    *B = *D;
}

// LD B, E
void opcode_43()
{
    *B = *E;
}

// LD B, H
void opcode_44()
{
    *B = *H;
}

// LD B, L
void opcode_45()
{
    *B = *L;
}

// LD B, (HL)
void opcode_46()
{
    *B = memory[*HL];
}

// LD B, A
void opcode_47()
{
    *B = *A;
}

// LD C, B
void opcode_48()
{
    *C = *B;
}

// LD C, C
void opcode_49()
{
    *C = *C;
}

// LD C, D
void opcode_4A()
{
    *C = *D;
}

// LD C, E
void opcode_4B()
{
    *C = *E;
}

// LD C, H
void opcode_4C()
{
    *C = *H;
}

// LD C, L
void opcode_4D()
{
    *C = *L;
}

// LD C, (HL)
void opcode_4E()
{
    *C = memory[*HL];
}

// LD C, A
void opcode_4F()
{
    *C = *A;
}

// LD D, B
void opcode_50()
{
    *D = *B;
}

// LD D, C
void opcode_51()
{
    *D = *C;
}

// LD D, D
void opcode_52()
{
    *D = *D;
}

// LD D, E
void opcode_53()
{
    *D = *E;
}

// LD D, H
void opcode_54()
{
    *D = *H;
}

// LD D, L
void opcode_55()
{
    *D = *L;
}

// LD D, (HL)
void opcode_56()
{
    *D = memory[*HL];
}

// LD D, A
void opcode_57()
{
    *D = *A;
}

// LD E, B
void opcode_58()
{
    *E = *B;
}

// LD E, C
void opcode_59()
{
    *E = *C;
}

// LD E, D
void opcode_5A()
{
    *E = *D;
}

// LD E, E
void opcode_5B()
{
    *E = *E;
}

// LD E, H
void opcode_5C()
{
    *E = *H;
}

// LD E, L
void opcode_5D()
{
    *E = *L;
}

// LD E, (HL)
void opcode_5E()
{
    *E = memory[*HL];
}

// LD E, A
void opcode_5F()
{
    *E = *A;
}

// LD H, B
void opcode_60()
{
    *H = *B;
}

// LD H, C
void opcode_61()
{
    *H = *C;
}

// LD H, D
void opcode_62()
{
    *H = *D;
}

// LD H, E
void opcode_63()
{
    *H = *E;
}

// LD H, H
void opcode_64()
{
    *H = *H;
}

// LD H, L
void opcode_65()
{
    *H = *L;
}

// LD H, (HL)
void opcode_66()
{
    *H = memory[*HL];
}

// LD H, A
void opcode_67()
{
    *H = *A;
}

// LD L, B
void opcode_68()
{
    *L = *B;
}

// LD L, C
void opcode_69()
{
    *L = *C;
}

// LD L, D
void opcode_6A()
{
    *L = *D;
}

// LD L, E
void opcode_6B()
{
    *L = *E;
}

// LD L, H
void opcode_6C()
{
    *L = *H;
}

// LD L, L
void opcode_6D()
{
    *L = *L;
}

// LD L, (HL)
void opcode_6E()
{
    *L = memory[*HL];
}

// LD L, A
void opcode_6F()
{
    *L = *A;
}

// LD (HL), B
void opcode_70()
{
    memory[*HL] = *B;
}

// LD (HL), C
void opcode_71()
{
    memory[*HL] = *C;
}

// LD (HL), D
void opcode_72()
{
    memory[*HL] = *D;
}

// LD (HL), E
void opcode_73()
{
    memory[*HL] = *E;
}

// LD (HL), H
void opcode_74()
{
    memory[*HL] = *H;
}

// LD (HL), L
void opcode_75()
{
    memory[*HL] = *L;
}

// HALT TODO
void opcode_76()
{

}

// LD (HL), A
void opcode_77()
{
    memory[*HL] = *A;
}

// LD A, B
void opcode_78()
{
    *A = *B;
}

// LD A, C
void opcode_79()
{
    *A = *C;
}

// LD A, D
void opcode_7A()
{
    *A = *D;
}

// LD A, E
void opcode_7B()
{
    *A = *E;
}

// LD A, H
void opcode_7C()
{
    *A = *H;
}

// LD A, L
void opcode_7D()
{
    *A = *L;
}

// LD A, (HL)
void opcode_7E()
{
    *A = memory[*HL];
}

// LD A, A
void opcode_7F()
{
    *A = *A;
}

// ADD A, B
void opcode_80()
{
    if ((((*A & 0x0F) + (*B & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *B;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *B;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, C
void opcode_81()
{
    if ((((*A & 0x0F) + (*C & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *C;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *C;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, D
void opcode_82()
{
    if ((((*A & 0x0F) + (*D & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *D;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *D;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, E
void opcode_83()
{
    if ((((*A & 0x0F) + (*E & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *E;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *E;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, H
void opcode_84()
{
    if ((((*A & 0x0F) + (*H & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *H;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *H;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, L
void opcode_85()
{
    if ((((*A & 0x0F) + (*L & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *L;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *L;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, (HL)
void opcode_86()
{
    if ((((*A & 0x0F) + (memory[*HL] & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = memory[*HL];
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += memory[*HL];
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADD A, A
void opcode_87()
{
    if ((((*A & 0x0F) + (*A & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *A;
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += *A;
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, B
void opcode_88()
{
    if ((((*A & 0x0F) + (*B & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *B;
    unsigned int temp_byte3 = test_c(*F);
    *A += *B + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, C
void opcode_89()
{
    if ((((*A & 0x0F) + (*C & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *C;
    unsigned int temp_byte3 = test_c(*F);
    *A += *C + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, D
void opcode_8A()
{
    if ((((*A & 0x0F) + (*D & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *D;
    unsigned int temp_byte3 = test_c(*F);
    *A += *D + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, E
void opcode_8B()
{
    if ((((*A & 0x0F) + (*E & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *E;
    unsigned int temp_byte3 = test_c(*F);
    *A += *E + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, H
void opcode_8C()
{
    if ((((*A & 0x0F) + (*H & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *H;
    unsigned int temp_byte3 = test_c(*F);
    *A += *H + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, L
void opcode_8D()
{
    if ((((*A & 0x0F) + (*L & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *L;
    unsigned int temp_byte3 = test_c(*F);
    *A += *L + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, (HL)
void opcode_8E()
{
    if ((((*A & 0x0F) + (memory[*HL] & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = memory[*HL];
    unsigned int temp_byte3 = test_c(*F);
    *A += memory[*HL] + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// ADC A, A
void opcode_8F()
{
    if ((((*A & 0x0F) + (*A & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = *A;
    unsigned int temp_byte3 = test_c(*F);
    *A += *A + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
}

// SUB B
void opcode_90()
{
    if ((*A & 0x0F) < (*B & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < *B) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= *B;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB C
void opcode_91()
{
    if ((*A & 0x0F) < (*C & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < *C) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= *C;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB D
void opcode_92()
{
    if ((*A & 0x0F) < (*D & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < *D) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= *D;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB E
void opcode_93()
{
    if ((*A & 0x0F) < (*E & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < *E) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= *E;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB H
void opcode_94()
{
    if ((*A & 0x0F) < (*H & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < *H) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= *H;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB L
void opcode_95()
{
    if ((*A & 0x0F) < (*L & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < *L) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= *L;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB (HL)
void opcode_96()
{
    if ((*A & 0x0F) < (memory[*HL] & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < memory[*HL]) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= memory[*HL];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SUB A
void opcode_97()
{
    *A -= *A;
    SET_Z(*F);
    SET_N(*F);
}

// SBC A, B
void opcode_98()
{
    if ((*A & 0x0F) < ((*B & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *B;
    int temp3 = test_c(*F);
    *A -= *B + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, C
void opcode_99()
{
    if ((*A & 0x0F) < ((*C & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *C;
    int temp3 = test_c(*F);
    *A -= *C + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, D
void opcode_9A()
{
    if ((*A & 0x0F) < ((*D & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *D;
    int temp3 = test_c(*F);
    *A -= *D + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, E
void opcode_9B()
{
    if ((*A & 0x0F) < ((*E & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *E;
    int temp3 = test_c(*F);
    *A -= *E + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, H
void opcode_9C()
{
    if ((*A & 0x0F) < ((*H & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *H;
    int temp3 = test_c(*F);
    *A -= *H + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, L
void opcode_9D()
{
    if ((*A & 0x0F) < ((*L & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *L;
    int temp3 = test_c(*F);
    *A -= *L + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, (HL)
void opcode_9E()
{
    if ((*A & 0x0F) < ((memory[*HL] & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = memory[*HL];
    int temp3 = test_c(*F);
    *A -= memory[*HL] + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// SBC A, A
void opcode_9F()
{
    if ((*A & 0x0F) < ((*A & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = *A;
    int temp3 = test_c(*F);
    *A -= *A + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
}

// AND B
void opcode_A0()
{
    *A = *A & *B;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND C
void opcode_A1()
{
    *A = *A & *C;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND D
void opcode_A2()
{
    *A = *A & *D;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND E
void opcode_A3()
{
    *A = *A & *E;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND H
void opcode_A4()
{
    *A = *A & *H;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND L
void opcode_A5()
{
    *A = *A & *L;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND (HL)
void opcode_A6()
{
    *A = *A & memory[*HL];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// AND A
void opcode_A7()
{
    *A = *A & *A;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
}

// XOR B
void opcode_A8()
{
    *A = *A ^ *B;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR C
void opcode_A9()
{
    *A = *A ^ *C;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR D
void opcode_AA()
{
    *A = *A ^ *D;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR E
void opcode_AB()
{
    *A = *A ^ *B;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR H
void opcode_AC()
{
    *A = *A ^ *H;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR L
void opcode_AD()
{
    *A = *A ^ *L;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR (HL)
void opcode_AE()
{
    *A = *A ^ memory[*HL];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// XOR A
void opcode_AF()
{
    *A = *A ^ *A;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR B
void opcode_B0()
{
    *A = *A | *B;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR C
void opcode_B1()
{
    *A = *A | *C;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR D
void opcode_B2()
{
    *A = *A | *D;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR E
void opcode_B3()
{
    *A = *A | *E;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR H
void opcode_B4()
{
    *A = *A | *H;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR L
void opcode_B5()
{
    *A = *A | *L;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR (HL)
void opcode_B6()
{
    *A = *A | memory[*HL];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// OR A
void opcode_B7()
{
    *A = *A | *A;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// CP B
void opcode_B8()
{
    if ((*A - *B) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < *B) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (*B & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP C
void opcode_B9()
{
    if ((*A - *C) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < *C) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (*C & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP D
void opcode_BA()
{
    if ((*A - *D) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < *D) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (*D & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP E
void opcode_BB()
{
    if ((*A - *E) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < *E) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (*E & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP H
void opcode_BC()
{
    if ((*A - *H) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < *H) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (*H & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP L
void opcode_BD()
{
    if ((*A - *L) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < *L) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (*L & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP (HL)
void opcode_BE()
{
    if ((*A - memory[*HL]) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < memory[*HL]) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (memory[*HL] & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// CP A
void opcode_BF()
{
    SET_Z(*F);
    RESET_C(*F);
    RESET_H(*F);
    SET_N(*F);
}

// RET NZ
void opcode_C0()
{
    if (test_z(*F) == 0) {
        pc.byte.L = memory[sp.SP++];
        pc.byte.H = memory[sp.SP++];
    } else {
        counter += 12;
    }
}

// POP BC
void opcode_C1()
{
    *C = memory[sp.SP++];
    *B = memory[sp.SP++];
}

// JP NZ, a16
void opcode_C2()
{
    if (test_z(*F) == 0) {
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
    if (test_z(*F) == 0) {
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
    memory[sp.SP-1] = *B;
    memory[sp.SP-2] = *C;
    sp.SP = sp.SP - 2;
}

// ADD A, d8
void opcode_C6()
{
    if ((((*A & 0x0F) + (memory[pc.PC] & 0x0F)) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = memory[pc.PC];
    if ((temp_byte1 + temp_byte2) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A += memory[pc.PC++];
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
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
    if (test_z(*F)) {
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
    if (test_z(*F)) {
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
    counter -= pre_cycles[opcode];
    switch (opcode) {
        case 0x00: pre_opcode_00(); break;
        case 0x01: pre_opcode_01(); break;
        case 0x02: pre_opcode_02(); break;
        case 0x03: pre_opcode_03(); break;
        case 0x04: pre_opcode_04(); break;
        case 0x05: pre_opcode_05(); break;
        case 0x06: pre_opcode_06(); break;
        case 0x07: pre_opcode_07(); break;
        case 0x08: pre_opcode_08(); break;
        case 0x09: pre_opcode_09(); break;
        case 0x0A: pre_opcode_0A(); break;
        case 0x0B: pre_opcode_0B(); break;
        case 0x0C: pre_opcode_0C(); break;
        case 0x0D: pre_opcode_0D(); break;
        case 0x0E: pre_opcode_0E(); break;
        case 0x0F: pre_opcode_0F(); break;
        case 0x10: pre_opcode_10(); break;
        case 0x11: pre_opcode_11(); break;
        case 0x12: pre_opcode_12(); break;
        case 0x13: pre_opcode_13(); break;
        case 0x14: pre_opcode_14(); break;
        case 0x15: pre_opcode_15(); break;
        case 0x16: pre_opcode_16(); break;
        case 0x17: pre_opcode_17(); break;
        case 0x18: pre_opcode_18(); break;
        case 0x19: pre_opcode_19(); break;
        case 0x1A: pre_opcode_1A(); break;
        case 0x1B: pre_opcode_1B(); break;
        case 0x1C: pre_opcode_1C(); break;
        case 0x1D: pre_opcode_1D(); break;
        case 0x1E: pre_opcode_1E(); break;
        case 0x1F: pre_opcode_1F(); break;
        case 0x20: pre_opcode_20(); break;
        case 0x21: pre_opcode_21(); break;
        case 0x22: pre_opcode_22(); break;
        case 0x23: pre_opcode_23(); break;
        case 0x24: pre_opcode_24(); break;
        case 0x25: pre_opcode_25(); break;
        case 0x26: pre_opcode_26(); break;
        case 0x27: pre_opcode_27(); break;
        case 0x28: pre_opcode_28(); break;
        case 0x29: pre_opcode_29(); break;
        case 0x2A: pre_opcode_2A(); break;
        case 0x2B: pre_opcode_2B(); break;
        case 0x2C: pre_opcode_2C(); break;
        case 0x2D: pre_opcode_2D(); break;
        case 0x2E: pre_opcode_2E(); break;
        case 0x2F: pre_opcode_2F(); break;
        case 0x30: pre_opcode_30(); break;
        case 0x31: pre_opcode_31(); break;
        case 0x32: pre_opcode_32(); break;
        case 0x33: pre_opcode_33(); break;
        case 0x34: pre_opcode_34(); break;
        case 0x35: pre_opcode_35(); break;
        case 0x36: pre_opcode_36(); break;
        case 0x37: pre_opcode_37(); break;
        case 0x38: pre_opcode_38(); break;
        case 0x39: pre_opcode_39(); break;
        case 0x3A: pre_opcode_3A(); break;
        case 0x3B: pre_opcode_3B(); break;
        case 0x3C: pre_opcode_3C(); break;
        case 0x3D: pre_opcode_3D(); break;
        case 0x3E: pre_opcode_3E(); break;
        case 0x3F: pre_opcode_3F(); break;
        case 0x40: pre_opcode_40(); break;
        case 0x41: pre_opcode_41(); break;
        case 0x42: pre_opcode_42(); break;
        case 0x43: pre_opcode_43(); break;
        case 0x44: pre_opcode_44(); break;
        case 0x45: pre_opcode_45(); break;
        case 0x46: pre_opcode_46(); break;
        case 0x47: pre_opcode_47(); break;
        case 0x48: pre_opcode_48(); break;
        case 0x49: pre_opcode_49(); break;
        case 0x4A: pre_opcode_4A(); break;
        case 0x4B: pre_opcode_4B(); break;
        case 0x4C: pre_opcode_4C(); break;
        case 0x4D: pre_opcode_4D(); break;
        case 0x4E: pre_opcode_4E(); break;
        case 0x4F: pre_opcode_4F(); break;
        case 0x50: pre_opcode_50(); break;
        case 0x51: pre_opcode_51(); break;
        case 0x52: pre_opcode_52(); break;
        case 0x53: pre_opcode_53(); break;
        case 0x54: pre_opcode_54(); break;
        case 0x55: pre_opcode_55(); break;
        case 0x56: pre_opcode_56(); break;
        case 0x57: pre_opcode_57(); break;
        case 0x58: pre_opcode_58(); break;
        case 0x59: pre_opcode_59(); break;
        case 0x5A: pre_opcode_5A(); break;
        case 0x5B: pre_opcode_5B(); break;
        case 0x5C: pre_opcode_5C(); break;
        case 0x5D: pre_opcode_5D(); break;
        case 0x5E: pre_opcode_5E(); break;
        case 0x5F: pre_opcode_5F(); break;
        case 0x60: pre_opcode_60(); break;
        case 0x61: pre_opcode_61(); break;
        case 0x62: pre_opcode_62(); break;
        case 0x63: pre_opcode_63(); break;
        case 0x64: pre_opcode_64(); break;
        case 0x65: pre_opcode_65(); break;
        case 0x66: pre_opcode_66(); break;
        case 0x67: pre_opcode_67(); break;
        case 0x68: pre_opcode_68(); break;
        case 0x69: pre_opcode_69(); break;
        case 0x6A: pre_opcode_6A(); break;
        case 0x6B: pre_opcode_6B(); break;
        case 0x6C: pre_opcode_6C(); break;
        case 0x6D: pre_opcode_6D(); break;
        case 0x6E: pre_opcode_6E(); break;
        case 0x6F: pre_opcode_6F(); break;
        case 0x70: pre_opcode_70(); break;
        case 0x71: pre_opcode_71(); break;
        case 0x72: pre_opcode_72(); break;
        case 0x73: pre_opcode_73(); break;
        case 0x74: pre_opcode_74(); break;
        case 0x75: pre_opcode_75(); break;
        case 0x76: pre_opcode_76(); break;
        case 0x77: pre_opcode_77(); break;
        case 0x78: pre_opcode_78(); break;
        case 0x79: pre_opcode_79(); break;
        case 0x7A: pre_opcode_7A(); break;
        case 0x7B: pre_opcode_7B(); break;
        case 0x7C: pre_opcode_7C(); break;
        case 0x7D: pre_opcode_7D(); break;
        case 0x7E: pre_opcode_7E(); break;
        case 0x7F: pre_opcode_7F(); break;
        case 0x80: pre_opcode_80(); break;
        case 0x81: pre_opcode_81(); break;
        case 0x82: pre_opcode_82(); break;
        case 0x83: pre_opcode_83(); break;
        case 0x84: pre_opcode_84(); break;
        case 0x85: pre_opcode_85(); break;
        case 0x86: pre_opcode_86(); break;
        case 0x87: pre_opcode_87(); break;
        case 0x88: pre_opcode_88(); break;
        case 0x89: pre_opcode_89(); break;
        case 0x8A: pre_opcode_8A(); break;
        case 0x8B: pre_opcode_8B(); break;
        case 0x8C: pre_opcode_8C(); break;
        case 0x8D: pre_opcode_8D(); break;
        case 0x8E: pre_opcode_8E(); break;
        case 0x8F: pre_opcode_8F(); break;
        case 0x90: pre_opcode_90(); break;
        case 0x91: pre_opcode_91(); break;
        case 0x92: pre_opcode_92(); break;
        case 0x93: pre_opcode_93(); break;
        case 0x94: pre_opcode_94(); break;
        case 0x95: pre_opcode_95(); break;
        case 0x96: pre_opcode_96(); break;
        case 0x97: pre_opcode_97(); break;
        case 0x98: pre_opcode_98(); break;
        case 0x99: pre_opcode_99(); break;
        case 0x9A: pre_opcode_9A(); break;
        case 0x9B: pre_opcode_9B(); break;
        case 0x9C: pre_opcode_9C(); break;
        case 0x9D: pre_opcode_9D(); break;
        case 0x9E: pre_opcode_9E(); break;
        case 0x9F: pre_opcode_9F(); break;
        case 0xA0: pre_opcode_A0(); break;
        case 0xA1: pre_opcode_A1(); break;
        case 0xA2: pre_opcode_A2(); break;
        case 0xA3: pre_opcode_A3(); break;
        case 0xA4: pre_opcode_A4(); break;
        case 0xA5: pre_opcode_A5(); break;
        case 0xA6: pre_opcode_A6(); break;
        case 0xA7: pre_opcode_A7(); break;
        case 0xA8: pre_opcode_A8(); break;
        case 0xA9: pre_opcode_A9(); break;
        case 0xAA: pre_opcode_AA(); break;
        case 0xAB: pre_opcode_AB(); break;
        case 0xAC: pre_opcode_AC(); break;
        case 0xAD: pre_opcode_AD(); break;
        case 0xAE: pre_opcode_AE(); break;
        case 0xAF: pre_opcode_AF(); break;
        case 0xB0: pre_opcode_B0(); break;
        case 0xB1: pre_opcode_B1(); break;
        case 0xB2: pre_opcode_B2(); break;
        case 0xB3: pre_opcode_B3(); break;
        case 0xB4: pre_opcode_B4(); break;
        case 0xB5: pre_opcode_B5(); break;
        case 0xB6: pre_opcode_B6(); break;
        case 0xB7: pre_opcode_B7(); break;
        case 0xB8: pre_opcode_B8(); break;
        case 0xB9: pre_opcode_B9(); break;
        case 0xBA: pre_opcode_BA(); break;
        case 0xBB: pre_opcode_BB(); break;
        case 0xBC: pre_opcode_BC(); break;
        case 0xBD: pre_opcode_BD(); break;
        case 0xBE: pre_opcode_BE(); break;
        case 0xBF: pre_opcode_BF(); break;
        case 0xC0: pre_opcode_C0(); break;
        case 0xC1: pre_opcode_C1(); break;
        case 0xC2: pre_opcode_C2(); break;
        case 0xC3: pre_opcode_C3(); break;
        case 0xC4: pre_opcode_C4(); break;
        case 0xC5: pre_opcode_C5(); break;
        case 0xC6: pre_opcode_C6(); break;
        case 0xC7: pre_opcode_C7(); break;
        case 0xC8: pre_opcode_C8(); break;
        case 0xC9: pre_opcode_C9(); break;
        case 0xCA: pre_opcode_CA(); break;
        case 0xCB: pre_opcode_CB(); break;
        case 0xCC: pre_opcode_CC(); break;
        case 0xCD: pre_opcode_CD(); break;
        case 0xCE: pre_opcode_CE(); break;
        case 0xCF: pre_opcode_CF(); break;
        case 0xD0: pre_opcode_D0(); break;
        case 0xD1: pre_opcode_D1(); break;
        case 0xD2: pre_opcode_D2(); break;
        case 0xD3: pre_opcode_D3(); break;
        case 0xD4: pre_opcode_D4(); break;
        case 0xD5: pre_opcode_D5(); break;
        case 0xD6: pre_opcode_D6(); break;
        case 0xD7: pre_opcode_D7(); break;
        case 0xD8: pre_opcode_D8(); break;
        case 0xD9: pre_opcode_D9(); break;
        case 0xDA: pre_opcode_DA(); break;
        case 0xDB: pre_opcode_DB(); break;
        case 0xDC: pre_opcode_DC(); break;
        case 0xDD: pre_opcode_DD(); break;
        case 0xDE: pre_opcode_DE(); break;
        case 0xDF: pre_opcode_DF(); break;
        case 0xE0: pre_opcode_E0(); break;
        case 0xE1: pre_opcode_E1(); break;
        case 0xE2: pre_opcode_E2(); break;
        case 0xE3: pre_opcode_E3(); break;
        case 0xE4: pre_opcode_E4(); break;
        case 0xE5: pre_opcode_E5(); break;
        case 0xE6: pre_opcode_E6(); break;
        case 0xE7: pre_opcode_E7(); break;
        case 0xE8: pre_opcode_E8(); break;
        case 0xE9: pre_opcode_E9(); break;
        case 0xEA: pre_opcode_EA(); break;
        case 0xEB: pre_opcode_EB(); break;
        case 0xEC: pre_opcode_EC(); break;
        case 0xED: pre_opcode_ED(); break;
        case 0xEE: pre_opcode_EE(); break;
        case 0xEF: pre_opcode_EF(); break;
        case 0xF0: pre_opcode_F0(); break;
        case 0xF1: pre_opcode_F1(); break;
        case 0xF2: pre_opcode_F2(); break;
        case 0xF3: pre_opcode_F3(); break;
        case 0xF4: pre_opcode_F4(); break;
        case 0xF5: pre_opcode_F5(); break;
        case 0xF6: pre_opcode_F6(); break;
        case 0xF7: pre_opcode_F7(); break;
        case 0xF8: pre_opcode_F8(); break;
        case 0xF9: pre_opcode_F9(); break;
        case 0xFA: pre_opcode_FA(); break;
        case 0xFB: pre_opcode_FB(); break;
        case 0xFC: pre_opcode_FC(); break;
        case 0xFD: pre_opcode_FD(); break;
        case 0xFE: pre_opcode_FE(); break;
        case 0xFF: pre_opcode_FF(); break;
    }
}

// CALL Z, a16
void opcode_CC()
{
    if (test_z(*F)) {
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
    if ((((*A & 0x0F) + (memory[pc.PC] & 0x0F) + (test_c(*F))) & 0x10) == 0x10) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    unsigned int temp_byte1 = *A;
    unsigned int temp_byte2 = memory[pc.PC];
    unsigned int temp_byte3 = test_c(*F);
    *A += memory[pc.PC++] + test_c(*F);
    if ((temp_byte1 + temp_byte2 + temp_byte3) > 255) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
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
    if (test_c(*F) == 0) {
        pc.byte.L = memory[sp.SP++];
        pc.byte.H = memory[sp.SP++];
    } else {
        counter += 12;
    }
}

// POP DE
void opcode_D1()
{
    *E = memory[sp.SP++];
    *D = memory[sp.SP++];
}

// JP NC, a16
void opcode_D2()
{
    if (test_c(*F) == 0) {
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
    if (test_c(*F) == 0) {
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
    memory[sp.SP-1] = *D;
    memory[sp.SP-2] = *E;
    sp.SP = sp.SP - 2;
}

// SUB d8
void opcode_D6()
{
    if ((*A & 0x0F) < (memory[pc.PC] & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    if (*A < memory[pc.PC]) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    *A -= memory[pc.PC++];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
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
    if (test_c(*F)) {
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
    if (test_c(*F)) {
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
    if (test_c(*F)) {
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
    if ((*A & 0x0F) < ((memory[pc.PC] & 0x0F) + test_c(*F))) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    int temp1 = *A;
    int temp2 = memory[pc.PC];
    int temp3 = test_c(*F);
    *A -= memory[pc.PC++] + test_c(*F);
    if (temp1 < (temp2 + temp3)) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    SET_N(*F);
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
    memory[0xFF00+temp] = *A;
}

// POP HL
void opcode_E1()
{
    *L = memory[sp.SP++];
    *H = memory[sp.SP++];
}

// LD (C), A
void opcode_E2()
{
    memory[0xFF00+*C] = *A;
}

// PUSH HL
void opcode_E5()
{
    memory[sp.SP-1] = *H;
    memory[sp.SP-2] = *L;
    sp.SP = sp.SP - 2;
}

// AND d8
void opcode_E6()
{
    *A = *A & memory[pc.PC++];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
    RESET_C(*F);
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
    pc.PC = *HL;
}

// LD (a16), A
void opcode_EA()
{
    WORD lsb = memory[pc.PC++];
    WORD msb = memory[pc.PC++];
    msb = msb << 8;
    WORD temp = msb + lsb;
    memory[temp] = *A;
}

// XOR d8
void opcode_EE()
{
    *A = *A ^ memory[pc.PC++];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
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
    *A = memory[0xFF00+temp];
}

// POP AF
void opcode_F1()
{
    *F = memory[sp.SP];
    *A = memory[sp.SP+1];
    sp.SP = sp.SP + 2;
}

// LD A, (C)
void opcode_F2()
{
    *A = memory[0xFF00+*C];
}

// DI
void opcode_F3()
{
    enable_interrupt = 0;
}

// PUSH AF
void opcode_F5()
{
    memory[sp.SP-1] = *A;
    memory[sp.SP-2] = *F;
    sp.SP = sp.SP - 2;
}

// OR d8
void opcode_F6()
{
    *A = *A | memory[pc.PC++];
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
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
    sp.SP = *HL;
}

// LD A, (a16)
void opcode_FA()
{
    WORD lsb = memory[pc.PC++];
    WORD msb = memory[pc.PC++];
    msb = msb << 8;
    WORD temp = msb + lsb;
    *A = memory[temp];
}

// EI
void opcode_FB()
{
    enable_interrupt = 1;
}

// CP d8
void opcode_FE()
{
    if ((*A - memory[pc.PC]) == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    if (*A < memory[pc.PC]) {
        SET_C(*F);
    } else {
        RESET_C(*F);
    }
    if ((*A & 0x0F) < (memory[pc.PC++] & 0x0F)) {
        SET_H(*F);
    } else {
        RESET_H(*F);
    }
    SET_N(*F);
}

// RST 38H
void opcode_FF()
{
    memory[sp.SP-1] = pc.byte.H;
    memory[sp.SP-2] = pc.byte.L;
    sp.SP = sp.SP - 2;
    pc.PC = 0x0000 + 0x38;
}
