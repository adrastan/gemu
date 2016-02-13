#include "pre_opcodes.h"
#include "cpu.h"
#include "memory.h"
#include "registers.h"

// RLC B
void pre_opcode_00()
{
    if (*B == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*B >> 7) & 1; // get 7th bit
    *B = *B << 1; // shift left 1
    *B ^= (-bit ^ *B) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC C
void pre_opcode_01()
{
    if (*C == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*C >> 7) & 1; // get 7th bit
    *C = *C << 1; // shift left 1
    *C ^= (-bit ^ *C) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC D
void pre_opcode_02()
{
    if (*D == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*D >> 7) & 1; // get 7th bit
    *D = *D << 1; // shift left 1
    *D ^= (-bit ^ *D) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC E
void pre_opcode_03()
{
    if (*E == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*E >> 7) & 1; // get 7th bit
    *E = *E << 1; // shift left 1
    *E ^= (-bit ^ *E) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC H
void pre_opcode_04()
{
    if (*H == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*H >> 7) & 1; // get 7th bit
    *H = *H << 1; // shift left 1
    *H ^= (-bit ^ *H) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC L
void pre_opcode_05()
{
    if (*L == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*L >> 7) & 1; // get 7th bit
    *L = *L << 1; // shift left 1
    *L ^= (-bit ^ *L) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC (HL)
void pre_opcode_06()
{
    if (memory[*HL] == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (memory[*HL] >> 7) & 1; // get 7th bit
    memory[*HL] = memory[*HL] << 1; // shift left 1
    memory[*HL] ^= (-bit ^ memory[*HL]) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RLC A
void pre_opcode_07()
{
    if (*A == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*A >> 7) & 1; // get 7th bit
    *A = *A << 1; // shift left 1
    *A ^= (-bit ^ *A) & (1 << 0x00); // copy bit 7 to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC B
void pre_opcode_08()
{
    if (*B == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*B >> 0) & 1; // get 0th bit
    *B = *B >> 1; // shift right 1
    *B ^= (-bit ^ *B) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC C
void pre_opcode_09()
{
    if (*C == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*C >> 0) & 1; // get 0th bit
    *C = *C >> 1; // shift right 1
    *C ^= (-bit ^ *C) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC D
void pre_opcode_0A()
{
    if (*D == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*D >> 0) & 1; // get 0th bit
    *D = *D >> 1; // shift right 1
    *D ^= (-bit ^ *D) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC E
void pre_opcode_0B()
{
    if (*E == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*E >> 0) & 1; // get 0th bit
    *E = *E >> 1; // shift right 1
    *E ^= (-bit ^ *E) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC H
void pre_opcode_0C()
{
    if (*H == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*H >> 0) & 1; // get 0th bit
    *H = *H >> 1; // shift right 1
    *H ^= (-bit ^ *H) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC L
void pre_opcode_0D()
{
    if (*L == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*L >> 0) & 1; // get 0th bit
    *L = *L >> 1; // shift right 1
    *L ^= (-bit ^ *L) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC (HL)
void pre_opcode_0E()
{
    if (memory[*HL] == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (memory[*HL] >> 0) & 1; // get 0th bit
    memory[*HL] = memory[*HL] >> 1; // shift right 1
    memory[*HL ]^= (-bit ^ memory[*HL]) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RRC A
void pre_opcode_0F()
{
    if (*A == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        RESET_C(*F);
        return;
    }
    BYTE bit = (*A >> 0) & 1; // get 0th bit
    *A = *A >> 1; // shift right 1
    *A ^= (-bit ^ *A) & (1 << 0x07); // copy bit 0 to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL B
void pre_opcode_10()
{
    if (*B == 0x80 && test_c(*F) == 0) {
        *B = *B << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*B == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*B >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *B = *B << 1; // shift left 1
    *B ^= (-carry ^ *B) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL C
void pre_opcode_11()
{
    if (*C == 0x80 && test_c(*F) == 0) {
        *C = *C << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*C == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*C >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *C = *C << 1; // shift left 1
    *C ^= (-carry ^ *C) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL D
void pre_opcode_12()
{
    if (*D == 0x80 && test_c(*F) == 0) {
        *D = *D << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*D == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*D >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *D = *D << 1; // shift left 1
    *D ^= (-carry ^ *D) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL E
void pre_opcode_13()
{
    if (*E == 0x80 && test_c(*F) == 0) {
        *E = *E << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*E == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*E >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *E = *E << 1; // shift left 1
    *E ^= (-carry ^ *E) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL H
void pre_opcode_14()
{
    if (*H == 0x80 && test_c(*F) == 0) {
        *H = *H << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*H == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*H >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *H = *H << 1; // shift left 1
    *H ^= (-carry ^ *H) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL L
void pre_opcode_15()
{
    if (*L == 0x80 && test_c(*F) == 0) {
        *L = *L << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*L == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*L >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *L = *L << 1; // shift left 1
    *L ^= (-carry ^ *L) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL (HL)
void pre_opcode_16()
{
    if (memory[*HL] == 0x80 && test_c(*F) == 0) {
        memory[*HL] = memory[*HL] << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (memory[*HL] == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (memory[*HL] >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    memory[*HL] = memory[*HL] << 1; // shift left 1
    memory[*HL] ^= (-carry ^ memory[*HL]) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RL A
void pre_opcode_17()
{
    if (*A == 0x80 && test_c(*F) == 0) {
        *A = *A << 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*A == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*A >> 7) & 1; // get 7th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *A = *A << 1; // shift left 1
    *A ^= (-carry ^ *A) & (1 << 0x00); // copy carry bit to bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR B
void pre_opcode_18()
{
    if (*B == 0x01 && test_c(*F) == 0) {
        *B = *B >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*B == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*B >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *B = *B >> 1; // shift right 1
    *B ^= (-carry ^ *B) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR C
void pre_opcode_19()
{
    if (*C == 0x01 && test_c(*F) == 0) {
        *C = *C >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*C == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*C >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *C = *C >> 1; // shift right 1
    *C ^= (-carry ^ *C) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR D
void pre_opcode_1A()
{
    if (*D == 0x01 && test_c(*F) == 0) {
        *D = *D >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*D == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*D >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *D = *D >> 1; // shift right 1
    *D ^= (-carry ^ *D) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR E
void pre_opcode_1B()
{
    if (*E == 0x01 && test_c(*F) == 0) {
        *E = *E >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*E == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*E >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *E = *E >> 1; // shift right 1
    *E ^= (-carry ^ *E) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR H
void pre_opcode_1C()
{
    if (*H == 0x01 && test_c(*F) == 0) {
        *H = *H >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*H == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*H >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *H = *H >> 1; // shift right 1
    *H ^= (-carry ^ *H) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR L
void pre_opcode_1D()
{
    if (*L == 0x01 && test_c(*F) == 0) {
        *L = *L >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*L == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*L >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *L = *L >> 1; // shift right 1
    *L ^= (-carry ^ *L) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR (HL)
void pre_opcode_1E()
{
    if (memory[*HL] == 0x01 && test_c(*F) == 0) {
        memory[*HL] = memory[*HL] >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (memory[*HL] == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (memory[*HL] >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    memory[*HL] = memory[*HL] >> 1; // shift right 1
    memory[*HL] ^= (-carry ^ memory[*HL]) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// RR A
void pre_opcode_1F()
{
    if (*A == 0x01 && test_c(*F) == 0) {
        *A = *A >> 1;
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        SET_C(*F);
        return;
    } else if (*A == 0 && test_c(*F) == 0) {
        SET_Z(*F);
        RESET_N(*F);
        RESET_H(*F);
        return;
    }
    BYTE bit = (*A >> 0) & 1; // get 0th bit
    BYTE carry = (*F >> 4) & 1; // get carry bit
    *A = *A >> 1; // shift right 1
    *A ^= (-carry ^ *A) & (1 << 0x07); // copy carry bit to bit 7
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    RESET_Z(*F); // reset Z flag
    RESET_N(*F); // reset N flag
    RESET_H(*F); // reset H flag
}

// SLA B
void pre_opcode_20()
{
    BYTE bit = (*B >> 7) & 1; // get 7th bit
    *B = *B << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*B == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA C
void pre_opcode_21()
{
    BYTE bit = (*C >> 7) & 1; // get 7th bit
    *C = *C << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*C == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA D
void pre_opcode_22()
{
    BYTE bit = (*D >> 7) & 1; // get 7th bit
    *D = *D << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*D == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA E
void pre_opcode_23()
{
    BYTE bit = (*E >> 7) & 1; // get 7th bit
    *E = *E << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*E == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA H
void pre_opcode_24()
{
    BYTE bit = (*H >> 7) & 1; // get 7th bit
    *H = *H << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*H == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA L
void pre_opcode_25()
{
    BYTE bit = (*L >> 7) & 1; // get 7th bit
    *L = *L << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*L == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA (HL)
void pre_opcode_26()
{
    BYTE bit = (memory[*HL] >> 7) & 1; // get 7th bit
    memory[*HL] = memory[*HL] << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (memory[*HL] == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SLA A
void pre_opcode_27()
{
    BYTE bit = (*A >> 7) & 1; // get 7th bit
    *A = *A << 1;
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
}

// SRA B
void pre_opcode_28()
{
    BYTE bit = (*B >> 7) & 1; // get 7th bit
    *B = *B >> 1;
    *B ^= (-bit ^ *B) & (1 << 0x07); // copy bit 7 back
    if (*B == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA C
void pre_opcode_29()
{
    BYTE bit = (*C >> 7) & 1; // get 7th bit
    *C = *C >> 1;
    *C ^= (-bit ^ *C) & (1 << 0x07); // copy bit 7 back
    if (*C == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA D
void pre_opcode_2A()
{
    BYTE bit = (*D >> 7) & 1; // get 7th bit
    *D = *D >> 1;
    *D ^= (-bit ^ *D) & (1 << 0x07); // copy bit 7 back
    if (*D == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA E
void pre_opcode_2B()
{
    BYTE bit = (*E >> 7) & 1; // get 7th bit
    *E = *E >> 1;
    *E ^= (-bit ^ *E) & (1 << 0x07); // copy bit 7 back
    if (*E == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA H
void pre_opcode_2C()
{
    BYTE bit = (*H >> 7) & 1; // get 7th bit
    *H = *H >> 1;
    *H ^= (-bit ^ *H) & (1 << 0x07); // copy bit 7 back
    if (*H == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA L
void pre_opcode_2D()
{
    BYTE bit = (*L >> 7) & 1; // get 7th bit
    *L = *L >> 1;
    *L ^= (-bit ^ *L) & (1 << 0x07); // copy bit 7 back
    if (*L == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA (HL)
void pre_opcode_2E()
{
    BYTE bit = (memory[*HL] >> 7) & 1; // get 7th bit
    memory[*HL] = memory[*HL] >> 1;
    memory[*HL] ^= (-bit ^ memory[*HL]) & (1 << 0x07); // copy bit 7 back
    if (memory[*HL] == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SRA A
void pre_opcode_2F()
{
    BYTE bit = (*A >> 7) & 1; // get 7th bit
    *A = *A >> 1;
    *A ^= (-bit ^ *A) & (1 << 0x07); // copy bit 7 back
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_C(*F);
}

// SWAP B
void pre_opcode_30()
{
    BYTE low_nibble = 0x0F & *B;
    BYTE high_nibble = 0xF0 & *B;
    *B = high_nibble + low_nibble;
    if (*B == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP C
void pre_opcode_31()
{
    BYTE low_nibble = 0x0F & *C;
    BYTE high_nibble = 0xF0 & *C;
    *C = high_nibble + low_nibble;
    if (*C == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP D
void pre_opcode_32()
{
    BYTE low_nibble = 0x0F & *D;
    BYTE high_nibble = 0xF0 & *D;
    *D = high_nibble + low_nibble;
    if (*D == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP E
void pre_opcode_33()
{
    BYTE low_nibble = 0x0F & *E;
    BYTE high_nibble = 0xF0 & *E;
    *E = high_nibble + low_nibble;
    if (*E == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP H
void pre_opcode_34()
{
    BYTE low_nibble = 0x0F & *H;
    BYTE high_nibble = 0xF0 & *H;
    *H = high_nibble + low_nibble;
    if (*H == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP L
void pre_opcode_35()
{
    BYTE low_nibble = 0x0F & *L;
    BYTE high_nibble = 0xF0 & *L;
    *L = high_nibble + low_nibble;
    if (*L == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP (HL)
void pre_opcode_36()
{
    BYTE low_nibble = 0x0F & memory[*HL];
    BYTE high_nibble = 0xF0 & memory[*HL];
    memory[*HL] = high_nibble + low_nibble;
    if (memory[*HL] == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SWAP A
void pre_opcode_37()
{
    BYTE low_nibble = 0x0F & *A;
    BYTE high_nibble = 0xF0 & *A;
    *A = high_nibble + low_nibble;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
    RESET_C(*F);
}

// SRL B
void pre_opcode_38()
{
    BYTE bit = 0x01 & *B; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *B = *B >> 1;
    if (*B == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL C
void pre_opcode_39()
{
    BYTE bit = 0x01 & *C; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *C = *C >> 1;
    if (*C == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL D
void pre_opcode_3A()
{
    BYTE bit = 0x01 & *D; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *D = *D >> 1;
    if (*D == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL E
void pre_opcode_3B()
{
    BYTE bit = 0x01 & *E; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *E = *E >> 1;
    if (*E == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL H
void pre_opcode_3C()
{
    BYTE bit = 0x01 & *H; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *H = *H >> 1;
    if (*H == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL L
void pre_opcode_3D()
{
    BYTE bit = 0x01 & *L; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *L = *L >> 1;
    if (*L == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL (HL)
void pre_opcode_3E()
{
    BYTE bit = 0x01 & memory[*HL]; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    memory[*HL] = memory[*HL] >> 1;
    if (memory[*HL] == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// SRL A
void pre_opcode_3F()
{
    BYTE bit = 0x01 & *A; // get bit 0
    *F ^= (-bit ^ *F) & (1 << 0x04); // copy bit 0 to carry bit
    *A = *A >> 1;
    if (*A == 0) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    RESET_H(*F);
}

// BIT 0,B
void pre_opcode_40()
{
    if ((*B & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,C
void pre_opcode_41()
{
    if ((*C & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,D
void pre_opcode_42()
{
    if ((*D & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,E
void pre_opcode_43()
{
    if ((*E & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,H
void pre_opcode_44()
{
    if ((*H & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,L
void pre_opcode_45()
{
    if ((*L & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,(HL)
void pre_opcode_46()
{
    if ((memory[*HL] & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 0,A
void pre_opcode_47()
{
    if ((*A & (0x01 << 0)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,B
void pre_opcode_48()
{
    if ((*B & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,C
void pre_opcode_49()
{
    if ((*C & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,D
void pre_opcode_4A()
{
    if ((*D & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,E
void pre_opcode_4B()
{
    if ((*E & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,H
void pre_opcode_4C()
{
    if ((*H & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,L
void pre_opcode_4D()
{
    if ((*L & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,(HL)
void pre_opcode_4E()
{
    if ((memory[*HL] & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 1,A
void pre_opcode_4F()
{
    if ((*A & (0x01 << 1)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,B
void pre_opcode_50()
{
    if ((*B & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,C
void pre_opcode_51()
{
    if ((*C & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,D
void pre_opcode_52()
{
    if ((*D & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,E
void pre_opcode_53()
{
    if ((*E & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,H
void pre_opcode_54()
{
    if ((*H & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,L
void pre_opcode_55()
{
    if ((*L & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,(HL)
void pre_opcode_56()
{
    if ((memory[*HL] & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 2,A
void pre_opcode_57()
{
    if ((*A & (0x01 << 2)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,B
void pre_opcode_58()
{
    if ((*B & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,C
void pre_opcode_59()
{
    if ((*C & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,D
void pre_opcode_5A()
{
    if ((*D & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,E
void pre_opcode_5B()
{
    if ((*E & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,H
void pre_opcode_5C()
{
    if ((*H & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,L
void pre_opcode_5D()
{
    if ((*L & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,(HL)
void pre_opcode_5E()
{
    if ((memory[*HL] & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 3,A
void pre_opcode_5F()
{
    if ((*A & (0x01 << 3)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,B
void pre_opcode_60()
{
    if ((*B & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,C
void pre_opcode_61()
{
    if ((*C & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,D
void pre_opcode_62()
{
    if ((*D & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,E
void pre_opcode_63()
{
    if ((*E & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,H
void pre_opcode_64()
{
    if ((*H & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,L
void pre_opcode_65()
{
    if ((*L & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,(HL)
void pre_opcode_66()
{
    if ((memory[*HL] & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 4,A
void pre_opcode_67()
{
    if ((*A & (0x01 << 4)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,B
void pre_opcode_68()
{
    if ((*B & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,C
void pre_opcode_69()
{
    if ((*C & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,D
void pre_opcode_6A()
{
    if ((*D & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,E
void pre_opcode_6B()
{
    if ((*E & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,H
void pre_opcode_6C()
{
    if ((*H & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,L
void pre_opcode_6D()
{
    if ((*L & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,(HL)
void pre_opcode_6E()
{
    if ((memory[*HL] & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 5,A
void pre_opcode_6F()
{
    if ((*A & (0x01 << 5)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,B
void pre_opcode_70()
{
    if ((*B & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,C
void pre_opcode_71()
{
    if ((*C & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,D
void pre_opcode_72()
{
    if ((*D & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,E
void pre_opcode_73()
{
    if ((*E & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,H
void pre_opcode_74()
{
    if ((*H & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,L
void pre_opcode_75()
{
    if ((*L & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,(HL)
void pre_opcode_76()
{
    if ((memory[*HL] & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 6,A
void pre_opcode_77()
{
    if ((*A & (0x01 << 6)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,B
void pre_opcode_78()
{
    if ((*B & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,C
void pre_opcode_79()
{
    if ((*C & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,D
void pre_opcode_7A()
{
    if ((*D & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,E
void pre_opcode_7B()
{
    if ((*E & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,H
void pre_opcode_7C()
{
    if ((*H & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,L
void pre_opcode_7D()
{
    if ((*L & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,(HL)
void pre_opcode_7E()
{
    if ((memory[*HL] & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// BIT 7,A
void pre_opcode_7F()
{
    if ((*A & (0x01 << 7)) == 0x00) {
        SET_Z(*F);
    } else {
        RESET_Z(*F);
    }
    RESET_N(*F);
    SET_H(*F);
}

// RES 0,B
void pre_opcode_80()
{
    *B &= ~(0x01 << 0);
}

// RES 0,C
void pre_opcode_81()
{
    *C &= ~(0x01 << 0);
}

// RES 0,D
void pre_opcode_82()
{
    *D &= ~(0x01 << 0);
}

// RES 0,E
void pre_opcode_83()
{
    *E &= ~(0x01 << 0);
}

// RES 0,H
void pre_opcode_84()
{
    *H &= ~(0x01 << 0);
}

// RES 0,L
void pre_opcode_85()
{
    *L &= ~(0x01 << 0);
}

// RES 0,(HL)
void pre_opcode_86()
{
    memory[*HL] &= ~(0x01 << 0);
}

// RES 0,A
void pre_opcode_87()
{
    *A &= ~(0x01 << 0);
}

// RES 1,B
void pre_opcode_88()
{
    *B &= ~(0x01 << 1);
}

// RES 1,C
void pre_opcode_89()
{
    *C &= ~(0x01 << 1);
}

// RES 1,D
void pre_opcode_8A()
{
    *D &= ~(0x01 << 1);
}

// RES 1,E
void pre_opcode_8B()
{
    *E &= ~(0x01 << 1);
}

// RES 1,H
void pre_opcode_8C()
{
    *H &= ~(0x01 << 1);
}

// RES 1,L
void pre_opcode_8D()
{
    *L &= ~(0x01 << 1);
}

// RES 1,(HL)
void pre_opcode_8E()
{
    memory[*HL] &= ~(0x01 << 1);
}

// RES 1,A
void pre_opcode_8F()
{
    *A &= ~(0x01 << 1);
}

// RES 2,B
void pre_opcode_90()
{
    *B &= ~(0x01 << 2);
}

// RES 2,C
void pre_opcode_91()
{
    *C &= ~(0x01 << 2);
}

// RES 2,D
void pre_opcode_92()
{
    *D &= ~(0x01 << 2);
}

// RES 2,E
void pre_opcode_93()
{
    *E &= ~(0x01 << 2);
}

// RES 2,H
void pre_opcode_94()
{
    *H &= ~(0x01 << 2);
}

// RES 2,L
void pre_opcode_95()
{
    *L &= ~(0x01 << 2);
}

// RES 2,(HL)
void pre_opcode_96()
{
    memory[*HL] &= ~(0x01 << 2);
}

// RES 2,A
void pre_opcode_97()
{
    *A &= ~(0x01 << 2);
}

// RES 3,B
void pre_opcode_98()
{
    *B &= ~(0x01 << 3);
}

// RES 3,C
void pre_opcode_99()
{
    *C &= ~(0x01 << 3);
}

// RES 3,D
void pre_opcode_9A()
{
    *D &= ~(0x01 << 3);
}

// RES 3,E
void pre_opcode_9B()
{
    *E &= ~(0x01 << 3);
}

// RES 3,H
void pre_opcode_9C()
{
    *H &= ~(0x01 << 3);
}

// RES 3,L
void pre_opcode_9D()
{
    *L &= ~(0x01 << 3);
}

// RES 3,(HL)
void pre_opcode_9E()
{
    memory[*HL] &= ~(0x01 << 3);
}

// RES 3,A
void pre_opcode_9F()
{
    *A &= ~(0x01 << 3);
}

// RES 4,B
void pre_opcode_A0()
{
    *B &= ~(0x01 << 4);
}

// RES 4,C
void pre_opcode_A1()
{
    *C &= ~(0x01 << 4);
}

// RES 4,D
void pre_opcode_A2()
{
    *D &= ~(0x01 << 4);
}

// RES 4,E
void pre_opcode_A3()
{
    *E &= ~(0x01 << 4);
}

// RES 4,H
void pre_opcode_A4()
{
    *H &= ~(0x01 << 4);
}

// RES 4,L
void pre_opcode_A5()
{
    *L &= ~(0x01 << 4);
}

// RES 4,(HL)
void pre_opcode_A6()
{
    memory[*HL] &= ~(0x01 << 4);
}

// RES 4,A
void pre_opcode_A7()
{
    *A &= ~(0x01 << 4);
}

// RES 5,B
void pre_opcode_A8()
{
    *B &= ~(0x01 << 5);
}

// RES 5,C
void pre_opcode_A9()
{
    *C &= ~(0x01 << 5);
}

// RES 5,D
void pre_opcode_AA()
{
    *D &= ~(0x01 << 5);
}

// RES 5,E
void pre_opcode_AB()
{
    *E &= ~(0x01 << 5);
}

// RES 5,H
void pre_opcode_AC()
{
    *H &= ~(0x01 << 5);
}

// RES 5,L
void pre_opcode_AD()
{
    *L &= ~(0x01 << 5);
}

// RES 5,(HL)
void pre_opcode_AE()
{
    memory[*HL] &= ~(0x01 << 5);
}

// RES 5,A
void pre_opcode_AF()
{
    *A &= ~(0x01 << 5);
}

// RES 6,B
void pre_opcode_B0()
{
    *B &= ~(0x01 << 6);
}

// RES 6,C
void pre_opcode_B1()
{
    *C &= ~(0x01 << 6);
}

// RES 6,D
void pre_opcode_B2()
{
    *D &= ~(0x01 << 6);
}

// RES 6,E
void pre_opcode_B3()
{
    *E &= ~(0x01 << 6);
}

// RES 6,H
void pre_opcode_B4()
{
    *H &= ~(0x01 << 6);
}

// RES 6,L
void pre_opcode_B5()
{
    *L &= ~(0x01 << 6);
}

// RES 6,(HL)
void pre_opcode_B6()
{
    memory[*HL] &= ~(0x01 << 6);
}

// RES 6,A
void pre_opcode_B7()
{
    *A &= ~(0x01 << 6);
}

// RES 7,B
void pre_opcode_B8()
{
    *B &= ~(0x01 << 7);
}

// RES 7,C
void pre_opcode_B9()
{
    *C &= ~(0x01 << 7);
}

// RES 7,D
void pre_opcode_BA()
{
    *D &= ~(0x01 << 7);
}

// RES 7,E
void pre_opcode_BB()
{
    *E &= ~(0x01 << 7);
}

// RES 7,H
void pre_opcode_BC()
{
    *H &= ~(0x01 << 7);
}

// RES 7,L
void pre_opcode_BD()
{
    *L &= ~(0x01 << 7);
}

// RES 7,(HL)
void pre_opcode_BE()
{
    memory[*HL] &= ~(0x01 << 7);
}

// RES 7,A
void pre_opcode_BF()
{
    *A &= ~(0x01 << 7);
}

// SET 0,B
void pre_opcode_C0()
{
    *B |= (0x01 << 0);
}

// SET 0,C
void pre_opcode_C1()
{
    *C |= (0x01 << 0);
}

// SET 0,D
void pre_opcode_C2()
{
    *D |= (0x01 << 0);
}

// SET 0,E
void pre_opcode_C3()
{
    *E |= (0x01 << 0);
}

// SET 0,H
void pre_opcode_C4()
{
    *H |= (0x01 << 0);
}

// SET 0,L
void pre_opcode_C5()
{
    *L |= (0x01 << 0);
}

// SET 0,(HL)
void pre_opcode_C6()
{
    memory[*HL] |= (0x01 << 0);
}

// SET 0,A
void pre_opcode_C7()
{
    *A |= (0x01 << 0);
}

// SET 1,B
void pre_opcode_C8()
{
    *B |= (0x01 << 1);
}

// SET 1,C
void pre_opcode_C9()
{
    *C |= (0x01 << 1);
}

// SET 1,D
void pre_opcode_CA()
{
    *D |= (0x01 << 1);
}

// SET 1,E
void pre_opcode_CB()
{
    *E |= (0x01 << 1);
}

// SET 1,H
void pre_opcode_CC()
{
    *H |= (0x01 << 1);
}

// SET 1,L
void pre_opcode_CD()
{
    *L |= (0x01 << 1);
}

// SET 1,(HL)
void pre_opcode_CE()
{
    memory[*HL] |= (0x01 << 1);
}

// SET 1,A
void pre_opcode_CF()
{
    *A |= (0x01 << 1);
}

// SET 2,B
void pre_opcode_D0()
{
    *B |= (0x01 << 2);
}

// SET 2,C
void pre_opcode_D1()
{
    *C |= (0x01 << 2);
}

// SET 2,D
void pre_opcode_D2()
{
    *D |= (0x01 << 2);
}

// SET 2,E
void pre_opcode_D3()
{
    *E |= (0x01 << 2);
}

// SET 2,H
void pre_opcode_D4()
{
    *H |= (0x01 << 2);
}

// SET 2,L
void pre_opcode_D5()
{
    *L |= (0x01 << 2);
}

// SET 2,(HL)
void pre_opcode_D6()
{
    memory[*HL] |= (0x01 << 2);
}

// SET 2,A
void pre_opcode_D7()
{
    *A |= (0x01 << 2);
}

// SET 3,B
void pre_opcode_D8()
{
    *B |= (0x01 << 3);
}

// SET 3,C
void pre_opcode_D9()
{
    *C |= (0x01 << 3);
}

// SET 3,D
void pre_opcode_DA()
{
    *D |= (0x01 << 3);
}

// SET 3,E
void pre_opcode_DB()
{
    *E |= (0x01 << 3);
}

// SET 3,H
void pre_opcode_DC()
{
    *H |= (0x01 << 3);
}

// SET 3,L
void pre_opcode_DD()
{
    *L |= (0x01 << 3);
}

// SET 3,(HL)
void pre_opcode_DE()
{
    memory[*HL] |= (0x01 << 3);
}

// SET 3,A
void pre_opcode_DF()
{
    *A |= ~(0x01 << 3);
}

// SET 4,B
void pre_opcode_E0()
{
    *B |= (0x01 << 4);
}

// SET 4,C
void pre_opcode_E1()
{
    *C |= (0x01 << 4);
}

// SET 4,D
void pre_opcode_E2()
{
    *D |= (0x01 << 4);
}

// SET 4,E
void pre_opcode_E3()
{
    *E |= (0x01 << 4);
}

// SET 4,H
void pre_opcode_E4()
{
    *H |= (0x01 << 4);
}

// SET 4,L
void pre_opcode_E5()
{
    *L |= (0x01 << 4);
}

// SET 4,(HL)
void pre_opcode_E6()
{
    memory[*HL] |= (0x01 << 4);
}

// SET 4,A
void pre_opcode_E7()
{
    *A |= (0x01 << 4);
}

// SET 5,B
void pre_opcode_E8()
{
    *B |= (0x01 << 5);
}

// SET 5,C
void pre_opcode_E9()
{
    *C |= (0x01 << 5);
}

// SET 5,D
void pre_opcode_EA()
{
    *D |= (0x01 << 5);
}

// SET 5,E
void pre_opcode_EB()
{
    *E |= (0x01 << 5);
}

// SET 5,H
void pre_opcode_EC()
{
    *H |= (0x01 << 5);
}

// SET 5,L
void pre_opcode_ED()
{
    *L |= (0x01 << 5);
}

// SET 5,(HL)
void pre_opcode_EE()
{
    memory[*HL] |= (0x01 << 5);
}

// SET 5,A
void pre_opcode_EF()
{
    *A |= (0x01 << 5);
}

// SET 6,B
void pre_opcode_F0()
{
    *B |= (0x01 << 6);
}

// SET 6,C
void pre_opcode_F1()
{
    *C |= (0x01 << 6);
}

// SET 6,D
void pre_opcode_F2()
{
    *D |= (0x01 << 6);
}

// SET 6,E
void pre_opcode_F3()
{
    *E |= (0x01 << 6);
}

// SET 6,H
void pre_opcode_F4()
{
    *H |= (0x01 << 6);
}

// SET 6,L
void pre_opcode_F5()
{
    *L |= (0x01 << 6);
}

// SET 6,(HL)
void pre_opcode_F6()
{
    memory[*HL] |= (0x01 << 6);
}

// SET 6,A
void pre_opcode_F7()
{
    *A |= (0x01 << 6);
}

// SET 7,B
void pre_opcode_F8()
{
    *B |= (0x01 << 7);
}

// SET 7,C
void pre_opcode_F9()
{
    *C |= (0x01 << 7);
}

// SET 7,D
void pre_opcode_FA()
{
    *D |= (0x01 << 7);
}

// SET 7,E
void pre_opcode_FB()
{
    *E |= (0x01 << 7);
}

// SET 7,H
void pre_opcode_FC()
{
    *H |= (0x01 << 7);
}

// SET 7,L
void pre_opcode_FD()
{
    *L |= (0x01 << 7);
}

// SET 7,(HL)
void pre_opcode_FE()
{
    memory[*HL] |= (0x01 << 7);
}

// SET 7,A
void pre_opcode_FF()
{
    *A |= (0x01 << 7);
}
