#include "opcodes.h"
#include "pre_opcodes.h"
#include "memory.h"
#include "cpu.h"
#include "registers.h"
#include <stdlib.h>

extern int counter;
extern int pre_cycles[];
extern int ime;
extern u_int8 opcode;

// LD BC, d16
void opcode_01()
{
    regs.byte.C = read_memory(pc.PC++);
    regs.byte.B = read_memory(pc.PC++);
}

// LD DE, d16
void opcode_11()
{
    regs.byte.E = read_memory(pc.PC++);
    regs.byte.D = read_memory(pc.PC++);
}

// LD HL, d16
void opcode_21()
{
    regs.byte.L = read_memory(pc.PC++);
    regs.byte.H = read_memory(pc.PC++);
}

// LD SP, d16
void opcode_31()
{
    sp.byte.L = read_memory(pc.PC++);
    sp.byte.H = read_memory(pc.PC++);
}

// LD (BC), A
void opcode_02()
{
    write_memory(regs.word.BC, regs.byte.A);
}

// LD (DE), A
void opcode_12()
{
    write_memory(regs.word.DE, regs.byte.A);
}

// LD (HL+), A
void opcode_22()
{
    write_memory(regs.word.HL++, regs.byte.A);
}

// LD (HL-), A
void opcode_32()
{
    write_memory(regs.word.HL--, regs.byte.A);
}

// LD B, d8
void opcode_06()
{
    regs.byte.B = read_memory(pc.PC++);
}

// LD D, d8
void opcode_16()
{
    regs.byte.D = read_memory(pc.PC++);
}

// LD H, d8
void opcode_26()
{
    regs.byte.H = read_memory(pc.PC++);
}

// LD (HL), d8
void opcode_36()
{
    write_memory(regs.word.HL, read_memory(pc.PC++));
}

// LD (a16), SP
void opcode_08()
{
    u_int16 lower = read_memory(pc.PC++);
    u_int16 higher = read_memory(pc.PC++);
    u_int16 address = (higher << 8) | lower;
    write_memory(address++, sp.byte.L);
    write_memory(address, sp.byte.H);
}

// LD A, (BC)
void opcode_0A()
{
    regs.byte.A = read_memory(regs.word.BC);
}

// LD A, (DE)
void opcode_1A()
{
    regs.byte.A = read_memory(regs.word.DE);
}

// LD A, (HL+)
void opcode_2A()
{
    regs.byte.A = read_memory(regs.word.HL++);
}

// LD A, (HL-)
void opcode_3A()
{
    regs.byte.A = read_memory(regs.word.HL--);
}

// LD C, d8
void opcode_0E()
{
    regs.byte.C = read_memory(pc.PC++);
}

// LD E, d8
void opcode_1E()
{
    regs.byte.E = read_memory(pc.PC++);
}

// LD L, d8
void opcode_2E()
{
    regs.byte.L = read_memory(pc.PC++);
}

// LD A, d8
void opcode_3E()
{
    regs.byte.A = read_memory(pc.PC++);
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
    regs.byte.B = read_memory(regs.word.HL);
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
    regs.byte.C = read_memory(regs.word.HL);
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
    regs.byte.D = read_memory(regs.word.HL);
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
    regs.byte.E = read_memory(regs.word.HL);
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
    regs.byte.H = read_memory(regs.word.HL);
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
    regs.byte.L = read_memory(regs.word.HL);
}

// LD L, A
void opcode_6F()
{
    regs.byte.L = regs.byte.A;
}

// LD (HL), B
void opcode_70()
{
    write_memory(regs.word.HL, regs.byte.B);
}

// LD (HL), C
void opcode_71()
{
    write_memory(regs.word.HL, regs.byte.C);
}

// LD (HL), D
void opcode_72()
{
    write_memory(regs.word.HL, regs.byte.D);
}

// LD (HL), E
void opcode_73()
{
    write_memory(regs.word.HL, regs.byte.E);
}

// LD (HL), H
void opcode_74()
{
    write_memory(regs.word.HL, regs.byte.H);
}

// LD (HL), L
void opcode_75()
{
    write_memory(regs.word.HL, regs.byte.L);
}

// LD (HL), A
void opcode_77()
{
    write_memory(regs.word.HL, regs.byte.A);
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
    regs.byte.A = read_memory(regs.word.HL);
}

// LD A, A
void opcode_7F()
{
    regs.byte.A = regs.byte.A;
}

// LDH (a8), A
void opcode_E0()
{
    u_int8 temp = read_memory(pc.PC++);
    write_memory(0xFF00+temp, regs.byte.A);
}

// LDH A, (a8)
void opcode_F0()
{
    u_int8 temp = read_memory(pc.PC++);
    regs.byte.A = read_memory(0xFF00+temp);
}

// LD (C), A
void opcode_E2()
{
    write_memory(0xFF00+regs.byte.C, regs.byte.A);
}

// LD A, (C)
void opcode_F2()
{
    regs.byte.A = read_memory(0xFF00+regs.byte.C);
}

// LD SP, HL
void opcode_F9()
{
    sp.SP = regs.word.HL;
}

// LD (a16), A
void opcode_EA()
{
    u_int16 lsb = read_memory(pc.PC++);
    u_int16 msb = read_memory(pc.PC++);
    msb <<= 8;
    u_int16 address = msb | lsb;
    write_memory(address, regs.byte.A);
}

// LD A, (a16)
void opcode_FA()
{
    u_int16 lsb = read_memory(pc.PC++);
    u_int16 msb = read_memory(pc.PC++);
    msb <<= 8;
    u_int16 address = msb | lsb;
    regs.byte.A = read_memory(address);
}

// JR NZ, r8
void opcode_20()
{
    if (test_z() == 0) {
        u_int8 byte = read_memory(pc.PC++);
        pc.PC += (signed char)byte;
    } else {
        pc.PC++;
        counter += 4;
    }
}

// JR NC, r8
void opcode_30()
{
    if (test_c() == 0) {
        u_int8 byte = read_memory(pc.PC++);
        pc.PC += (signed char)byte;
    } else {
        pc.PC++;
        counter += 4;
    }
}

// JR r8
void opcode_18()
{
    u_int8 byte = read_memory(pc.PC++);
    pc.PC += (signed char)byte;
}

// JR Z, r8
void opcode_28()
{
    if (test_z()) {
        u_int8 byte = read_memory(pc.PC++);
        pc.PC += (signed char)byte;
    } else {
        pc.PC++;
        counter += 4;
    }
}

// JR C, r8
void opcode_38()
{
    if (test_c()) {
        u_int8 byte = read_memory(pc.PC++);
        pc.PC += (signed char)byte;
    } else {
        pc.PC++;
        counter += 4;
    }
}

// RET NZ
void opcode_C0()
{
    if (test_z() == 0) {
        pc.byte.L = read_memory(sp.SP);
        pc.byte.H = read_memory(sp.SP+1);
        sp.SP += 2;
    } else {
        counter += 12;
    }
}

// RET NC
void opcode_D0()
{
    if (test_c() == 0) {
        pc.byte.L = read_memory(sp.SP);
        pc.byte.H = read_memory(sp.SP+1);
        sp.SP += 2;
    } else {
        counter += 12;
    }
}

// JP NZ, a16
void opcode_C2()
{
    if (test_z() == 0) {
        u_int8 lsb = read_memory(pc.PC);
        u_int8 msb = read_memory(pc.PC+1);
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 4;
    }
}

// JP NC, a16
void opcode_D2()
{
    if (test_c() == 0) {
        u_int8 lsb = read_memory(pc.PC);
        u_int8 msb = read_memory(pc.PC+1);
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 4;
    }
}

// JP a16
void opcode_C3()
{
    u_int8 lsb = read_memory(pc.PC);
    u_int8 msb = read_memory(pc.PC+1);
    pc.byte.L = lsb;
    pc.byte.H = msb;
}

// CALL NZ, a16
void opcode_C4()
{
    if (test_z() == 0) {
        u_int8 lsb = read_memory(pc.PC++);
        u_int8 msb = read_memory(pc.PC++);
        write_memory(sp.SP-1, pc.byte.H);
        write_memory(sp.SP-2, pc.byte.L);
        sp.SP -= 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 12;
    }
}

// CALL NC, a16
void opcode_D4()
{
    if (test_c() == 0) {
        u_int8 lsb = read_memory(pc.PC++);
        u_int8 msb = read_memory(pc.PC++);
        write_memory(sp.SP-1, pc.byte.H);
        write_memory(sp.SP-2, pc.byte.L);
        sp.SP -= 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 12;
    }
}

// RST 00H
void opcode_C7()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x00;
}

// RST 10H
void opcode_D7()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x10;
}

// RST 20H
void opcode_E7()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x20;
}

// RST 30H
void opcode_F7()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x30;
}

// RET Z
void opcode_C8()
{
    if (test_z()) {
        pc.byte.L = read_memory(sp.SP++);
        pc.byte.H = read_memory(sp.SP++);
    } else {
        counter += 12;
    }
}

// RET C
void opcode_D8()
{
    if (test_c()) {
        pc.byte.L = read_memory(sp.SP++);
        pc.byte.H = read_memory(sp.SP++);
    } else {
        counter += 12;
    }
}

// RET
void opcode_C9()
{
    pc.byte.L = read_memory(sp.SP++);
    pc.byte.H = read_memory(sp.SP++);
}

// RETI
void opcode_D9()
{
    pc.byte.L = read_memory(sp.SP++);
    pc.byte.H = read_memory(sp.SP++);
    ime = 1;
}

// JP (HL)
void opcode_E9()
{
    pc.PC = regs.word.HL;
}

// JP Z, a16
void opcode_CA()
{
    if (test_z()) {
        u_int8 lsb = read_memory(pc.PC);
        u_int8 msb = read_memory(pc.PC+1);
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 4;
    }
}

// JP C, a16
void opcode_DA()
{
    if (test_c()) {
        u_int8 lsb = read_memory(pc.PC);
        u_int8 msb = read_memory(pc.PC+1);
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 4;
    }
}

// CALL Z, a16
void opcode_CC()
{
    if (test_z()) {
        u_int8 lsb = read_memory(pc.PC++);
        u_int8 msb = read_memory(pc.PC++);
        write_memory(sp.SP-1, pc.byte.H);
        write_memory(sp.SP-2, pc.byte.L);
        sp.SP -= 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 12;
    }
}

// CALL C, a16
void opcode_DC()
{
    if (test_c()) {
        u_int8 lsb = read_memory(pc.PC++);
        u_int8 msb = read_memory(pc.PC++);
        write_memory(sp.SP-1, pc.byte.H);
        write_memory(sp.SP-2, pc.byte.L);
        sp.SP -= 2;
        pc.byte.L = lsb;
        pc.byte.H = msb;
    } else {
        pc.PC += 2;
        counter += 12;
    }
}

// CALL a16
void opcode_CD()
{
    u_int8 lsb = read_memory(pc.PC++);
    u_int8 msb = read_memory(pc.PC++);
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.byte.L = lsb;
    pc.byte.H = msb;
}

// RST 08H
void opcode_CF()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x08;
}

// RST 18H
void opcode_DF()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x18;
}

// RST 28H
void opcode_EF()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x28;
}

// RST 38H
void opcode_FF()
{
    write_memory(sp.SP-1, pc.byte.H);
    write_memory(sp.SP-2, pc.byte.L);
    sp.SP -= 2;
    pc.PC = 0x0000 + 0x38;
}

// INC BC
void opcode_03()
{
    ++regs.word.BC;
}

// INC DE
void opcode_13()
{
    ++regs.word.DE;
}

// INC HL
void opcode_23()
{
    ++regs.word.HL;
}

// INC SP
void opcode_33()
{
    ++sp.SP;
}

// INC B
void opcode_04()
{
    INC_8(&regs.byte.B);
}

// INC D
void opcode_14()
{
    INC_8(&regs.byte.D);
}

// INC H
void opcode_24()
{
    INC_8(&regs.byte.H);
}

// INC (HL)
void opcode_34()
{
    u_int8 byte = read_memory(regs.word.HL);
    INC_8(&byte);
    write_memory(regs.word.HL, byte);
}

// INC C
void opcode_0C()
{
    INC_8(&regs.byte.C);
}

// INC E
void opcode_1C()
{
    INC_8(&regs.byte.E);
}

// INC L
void opcode_2C()
{
    INC_8(&regs.byte.L);
}

// INC A
void opcode_3C()
{
    INC_8(&regs.byte.A);
}

// DEC B
void opcode_05()
{
    DEC_8(&regs.byte.B);
}

// DEC D
void opcode_15()
{
    DEC_8(&regs.byte.D);
}

// DEC H
void opcode_25()
{
    DEC_8(&regs.byte.H);
}

// DEC (HL)
void opcode_35()
{
    u_int8 byte = read_memory(regs.word.HL);
    DEC_8(&byte);
    write_memory(regs.word.HL, byte);
}

// DEC BC
void opcode_0B()
{
    --regs.word.BC;
}

// DEC DE
void opcode_1B()
{
    --regs.word.DE;
}

// DEC HL
void opcode_2B()
{
    --regs.word.HL;
}

// DEC SP
void opcode_3B()
{
    --sp.SP;
}

// DEC C
void opcode_0D()
{
    DEC_8(&regs.byte.C);
}

// DEC E
void opcode_1D()
{
    DEC_8(&regs.byte.E);
}

// DEC L
void opcode_2D()
{
    DEC_8(&regs.byte.L);
}

// DEC A
void opcode_3D()
{
    DEC_8(&regs.byte.A);
}

// RLCA
void opcode_07()
{
    u_int8 bit = (regs.byte.A >> 7) & 1; // get 7th bit
    regs.byte.A = regs.byte.A << 1; // shift left 1
    regs.byte.A |= bit;
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    reset_z();
    reset_h();
    reset_n();
}

// RLA
void opcode_17()
{
    u_int8 bit = (regs.byte.A >> 7) & 1; // get 7th bit
    u_int8 carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.A = regs.byte.A << 1; // shift left 1
    regs.byte.A |= carry;
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    reset_z();
    reset_n();
    reset_h();
}

// DAA
void opcode_27()
{
    u_int8 high_nibble = regs.byte.A >> 4;
    u_int8 low_nibble = regs.byte.A & 0xf;
    int a = regs.byte.A;
    if (test_n() == 0) {
        if (low_nibble > 0x09 || test_h() == 1) {
            a += 0x06;
        }
        if (high_nibble > 0x09 || test_c() == 1) {
            a += 0x60;
        }
    } else {
        if (test_h()) {
            a = (a - 0x06) & 0xff;
        }
        if (test_c()) {
            a -= 0x60;
        }
    }
    reset_h();
    if ((a & 0x100) == 0x100) {
        set_c();
    }
    regs.byte.A = (a & 0xff);
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
}

// SCF
void opcode_37()
{
    set_c();
    reset_h();
    reset_n();
}

// RRCA
void opcode_0F()
{
    u_int8 bit = (regs.byte.A) & 1; // get 0th bit
    regs.byte.A = regs.byte.A >> 1; // shift left 1
    regs.byte.A |= (bit << 7);
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    reset_z();
    reset_h();
    reset_n();
}

// RRA
void opcode_1F()
{
    u_int8 bit = (regs.byte.A) & 1; // get 0th bit
    u_int8 carry = (regs.byte.F >> 4) & 1; // get carry bit
    regs.byte.A = regs.byte.A >> 1; // shift left 1
    regs.byte.A |= (carry << 7);
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    reset_z();
    reset_n();
    reset_h();
}

// CPL
void opcode_2F()
{
    regs.byte.A = ~regs.byte.A;
    set_n();
    set_h();
}

// CCF
void opcode_3F()
{
    if (test_c()) {
        reset_c();
    } else {
        set_c();
    }
    reset_h();
    reset_n();
}

// ADD HL, BC
void opcode_09()
{
    ADD_HL(regs.word.BC);
}

// ADD HL, DE
void opcode_19()
{
    ADD_HL(regs.word.DE);
}

// ADD HL, HL
void opcode_29()
{
    ADD_HL(regs.word.HL);
}

// ADD HL, SP
void opcode_39()
{
    ADD_HL(sp.SP);
}

// ADD SP, r8
void opcode_E8()
{
    reset_z();
    reset_n();
    reset_h();
    reset_c();
    u_int8 n = read_memory(pc.PC++);
    if ((((sp.SP & 0xf) + (n & 0xf)) & 0x10) == 0x10) {
        set_h();
    }
    if ((((sp.SP & 0xff) + (n & 0xff)) & 0x0100) == 0x0100) {
        set_c();
    }
    sp.SP += (signed char)n;
}

// LD HL, SP+r8
void opcode_F8()
{
    reset_z();
    reset_n();
    reset_h();
    reset_c();
    u_int8 n = read_memory(pc.PC++);
    if ((((sp.SP & 0xf) + (n & 0xf)) & 0x10) == 0x10) {
        set_h();
    }
    if ((((sp.SP & 0xff) + (n & 0xff)) & 0x0100) == 0x0100) {
        set_c();
    }
    u_int16 address = sp.SP + (signed char)n;
    regs.word.HL = address;
}

// ADD A, B
void opcode_80()
{
    ADD_A(regs.byte.B);
}

// ADD A, C
void opcode_81()
{
    ADD_A(regs.byte.C);
}

// ADD A, D
void opcode_82()
{
    ADD_A(regs.byte.D);
}

// ADD A, E
void opcode_83()
{
    ADD_A(regs.byte.E);
}

// ADD A, H
void opcode_84()
{
    ADD_A(regs.byte.H);
}

// ADD A, L
void opcode_85()
{
    ADD_A(regs.byte.L);
}

// ADD A, (HL)
void opcode_86()
{
    ADD_A(read_memory(regs.word.HL));
}

// ADD A, A
void opcode_87()
{
    ADD_A(regs.byte.A);
}

// ADD A, d8
void opcode_C6()
{
    ADD_A(read_memory(pc.PC++));
}

// ADC A, B
void opcode_88()
{
    ADC_A(regs.byte.B);
}

// ADC A, C
void opcode_89()
{
    ADC_A(regs.byte.C);
}

// ADC A, D
void opcode_8A()
{
    ADC_A(regs.byte.D);
}

// ADC A, E
void opcode_8B()
{
    ADC_A(regs.byte.E);
}

// ADC A, H
void opcode_8C()
{
    ADC_A(regs.byte.H);
}

// ADC A, L
void opcode_8D()
{
    ADC_A(regs.byte.L);
}

// ADC A, (HL)
void opcode_8E()
{
   ADC_A(read_memory(regs.word.HL));
}

// ADC A, A
void opcode_8F()
{
    ADC_A(regs.byte.A);
}

// ADC A, d8
void opcode_CE()
{
    ADC_A(read_memory(pc.PC++));
}

// SUB B
void opcode_90()
{
    SUB_A(regs.byte.B);
}

// SUB C
void opcode_91()
{
    SUB_A(regs.byte.C);
}

// SUB D
void opcode_92()
{
    SUB_A(regs.byte.D);
}

// SUB E
void opcode_93()
{
    SUB_A(regs.byte.E);
}

// SUB H
void opcode_94()
{
    SUB_A(regs.byte.H);
}

// SUB L
void opcode_95()
{
    SUB_A(regs.byte.L);
}

// SUB (HL)
void opcode_96()
{
    SUB_A(read_memory(regs.word.HL));
}

// SUB A
void opcode_97()
{
    regs.byte.A = 0;
    reset_h();
    reset_c();
    set_z();
    set_n();
}

// SUB d8
void opcode_D6()
{
    SUB_A(read_memory(pc.PC++));
}

// SBC A, B
void opcode_98()
{
    SBC_A(regs.byte.B);
}

// SBC A, C
void opcode_99()
{
    SBC_A(regs.byte.C);
}

// SBC A, D
void opcode_9A()
{
    SBC_A(regs.byte.D);
}

// SBC A, E
void opcode_9B()
{
    SBC_A(regs.byte.E);
}

// SBC A, H
void opcode_9C()
{
    SBC_A(regs.byte.H);
}

// SBC A, L
void opcode_9D()
{
    SBC_A(regs.byte.L);
}

// SBC A, (HL)
void opcode_9E()
{
    SBC_A(read_memory(regs.word.HL));
}

// SBC A, A
void opcode_9F()
{
    SBC_A(regs.byte.A);
}

// SBC A, d8
void opcode_DE()
{
    SBC_A(read_memory(pc.PC++));
}

// AND B
void opcode_A0()
{
    AND_A(regs.byte.B);
}

// AND C
void opcode_A1()
{
    AND_A(regs.byte.C);
}

// AND D
void opcode_A2()
{
    AND_A(regs.byte.D);
}

// AND E
void opcode_A3()
{
    AND_A(regs.byte.E);
}

// AND H
void opcode_A4()
{
    AND_A(regs.byte.H);
}

// AND L
void opcode_A5()
{
    AND_A(regs.byte.L);
}

// AND (HL)
void opcode_A6()
{
    AND_A(read_memory(regs.word.HL));
}

// AND A
void opcode_A7()
{
    AND_A(regs.byte.A);
}

// AND d8
void opcode_E6()
{
    AND_A(read_memory(pc.PC++));
}

// XOR B
void opcode_A8()
{
    XOR_A(regs.byte.B);
}

// XOR C
void opcode_A9()
{
    XOR_A(regs.byte.C);
}

// XOR D
void opcode_AA()
{
    XOR_A(regs.byte.D);
}

// XOR E
void opcode_AB()
{
    XOR_A(regs.byte.E);
}

// XOR H
void opcode_AC()
{
    XOR_A(regs.byte.H);
}

// XOR L
void opcode_AD()
{
    XOR_A(regs.byte.L);
}

// XOR (HL)
void opcode_AE()
{
    XOR_A(read_memory(regs.word.HL));
}

// XOR A
void opcode_AF()
{
    XOR_A(regs.byte.A);
}

// XOR d8
void opcode_EE()
{
    XOR_A(read_memory(pc.PC++));
}

// OR B
void opcode_B0()
{
    OR_A(regs.byte.B);
}

// OR C
void opcode_B1()
{
    OR_A(regs.byte.C);
}

// OR D
void opcode_B2()
{
    OR_A(regs.byte.D);
}

// OR E
void opcode_B3()
{
    OR_A(regs.byte.E);
}

// OR H
void opcode_B4()
{
    OR_A(regs.byte.H);
}

// OR L
void opcode_B5()
{
    OR_A(regs.byte.L);
}

// OR (HL)
void opcode_B6()
{
    OR_A(read_memory(regs.word.HL));
}

// OR A
void opcode_B7()
{
    OR_A(regs.byte.A);
}

// OR d8
void opcode_F6()
{
    OR_A(read_memory(pc.PC++));
}

// CP B
void opcode_B8()
{
    CP_A(regs.byte.B);
}

// CP C
void opcode_B9()
{
    CP_A(regs.byte.C);
}

// CP D
void opcode_BA()
{
    CP_A(regs.byte.D);
}

// CP E
void opcode_BB()
{
    CP_A(regs.byte.E);
}

// CP H
void opcode_BC()
{
    CP_A(regs.byte.H);
}

// CP L
void opcode_BD()
{
    CP_A(regs.byte.L);
}

// CP (HL)
void opcode_BE()
{
    CP_A(read_memory(regs.word.HL));
}

// CP A
void opcode_BF()
{
    set_z();
    reset_h();
    reset_c();
    set_n();
}

// CP d8
void opcode_FE()
{
    CP_A(read_memory(pc.PC++));
}

// POP BC
void opcode_C1()
{
    regs.byte.C = read_memory(sp.SP++);
    regs.byte.B = read_memory(sp.SP++);
}

// POP DE
void opcode_D1()
{
    regs.byte.E = read_memory(sp.SP++);
    regs.byte.D = read_memory(sp.SP++);
}

// POP HL
void opcode_E1()
{
    regs.byte.L = read_memory(sp.SP++);
    regs.byte.H = read_memory(sp.SP++);
}

// POP AF
void opcode_F1()
{
    regs.byte.F = read_memory(sp.SP++);
    regs.byte.A = read_memory(sp.SP++);
}

// PUSH BC
void opcode_C5()
{
    write_memory(sp.SP-1, regs.byte.B);
    write_memory(sp.SP-2, regs.byte.C);
    sp.SP = sp.SP - 2;
}

// PUSH DE
void opcode_D5()
{
    write_memory(sp.SP-1, regs.byte.D);
    write_memory(sp.SP-2, regs.byte.E);
    sp.SP = sp.SP - 2;
}

// PUSH HL
void opcode_E5()
{
    write_memory(sp.SP-1, regs.byte.H);
    write_memory(sp.SP-2, regs.byte.L);
    sp.SP = sp.SP - 2;
}

// PUSH AF
void opcode_F5()
{
    write_memory(sp.SP-1, regs.byte.A);
    write_memory(sp.SP-2, regs.byte.F);
    sp.SP = sp.SP - 2;
}

// STOP 0
void opcode_10()
{
    pc.PC++;
    return;
}

// HALT TODO
void opcode_76()
{

}

// PREFIX
void opcode_CB()
{
    opcode = read_memory(pc.PC++);
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

// DI
void opcode_F3()
{
    ime = 0;
}

// EI
void opcode_FB()
{
    ime = 1;
}
