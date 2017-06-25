#ifndef OPCODES_H
#define OPCODES_H
#include "registers.h"
#include "cpu.h"

static inline void INC_8(u_int8 * byte)
{
    reset_n();
    if ((*byte & 0xf) == 0xf) {
        set_h();
    } else {
        reset_h();
    }
    ++(*byte);
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
}

static inline void DEC_8(u_int8 * byte)
{
    set_n();
    if ((*byte & 0xf) == 0) {
        set_h();
    } else {
        reset_h();
    }
    --(*byte);
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
}

static inline void ADD_HL(u_int16 word)
{
    reset_n();
    if ((((regs.word.HL & 0x0FFF) + (word & 0x0FFF)) & 0x1000) == 0x1000) {
        set_h();
    } else {
        reset_h();
    }
    int word1 = regs.word.HL;
    int word2 = word;
    if ((((word1 & 0xffff) + (word2 & 0xffff)) & 0x10000) == 0x10000) {
        set_c();
    } else {
        reset_c();
    }
    regs.word.HL += word;
}

static inline void ADD_A(u_int8 byte)
{
    if (half_carry(regs.byte.A, byte)) {
        set_h();
    } else {
        reset_h();
    }
    u_int16 temp_byte1 = regs.byte.A;
    u_int16 temp_byte2 = byte;
    if (((temp_byte1 + temp_byte2) & 0x0100) == 0x0100) {
        set_c();
    } else {
        reset_c();
    }
    regs.byte.A += byte;
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
}

static inline void ADC_A(u_int8 byte)
{
    int c = test_c();
    if (((regs.byte.A & 0x0f) + (byte & 0xf) + c) > 0xf) {
        set_h();
    } else {
        reset_h();
    }
    int result = regs.byte.A + byte + c;
    if (result > 0xff) {
        set_c();
    } else {
        reset_c();
    }
    regs.byte.A += byte + c;
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
}

static inline void SUB_A(u_int8 byte)
{
    if ((regs.byte.A & 0x0F) < (byte & 0x0F)) {
        set_h();
    } else {
        reset_h();
    }
    if (regs.byte.A < byte) {
        set_c();
    } else {
        reset_c();
    }
    regs.byte.A -= byte;
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    set_n();
}

static inline void SBC_A(u_int8 byte)
{
    //byte += test_c();
    u_int8 c = test_c();
    if ((u_int16)regs.byte.A < ((u_int16)byte + c)) {
        set_c();
    } else {
        reset_c();
    }
    if ((regs.byte.A & 0x0f) < ((byte &0x0f) + c)) {
        set_h();
    } else {
        reset_h();
    }
    regs.byte.A -= (byte + c);
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    set_n();
}

static inline void AND_A(u_int8 byte)
{
    regs.byte.A &= byte;
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    set_h();
    reset_c();
}

static inline void XOR_A(u_int8 byte)
{
    regs.byte.A ^= byte;
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
    reset_c();
}

static inline void OR_A(u_int8 byte)
{
    regs.byte.A |= byte;
    if (regs.byte.A == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
    reset_c();
}

static inline void CP_A(u_int8 byte)
{
    if ((regs.byte.A - byte) == 0) {
        set_z();
    } else {
        reset_z();
    }
    if (regs.byte.A < byte) {
        set_c();
    } else {
        reset_c();
    }
    if ((regs.byte.A & 0x0F) < (byte & 0x0F)) {
        set_h();
    } else {
        reset_h();
    }
    set_n();
}

void opcode_01();
void opcode_02();
void opcode_03();
void opcode_04();
void opcode_05();
void opcode_06();
void opcode_07();
void opcode_08();
void opcode_09();
void opcode_0A();
void opcode_0B();
void opcode_0C();
void opcode_0D();
void opcode_0E();
void opcode_0F();
void opcode_10();
void opcode_11();
void opcode_12();
void opcode_13();
void opcode_14();
void opcode_15();
void opcode_16();
void opcode_17();
void opcode_18();
void opcode_19();
void opcode_1A();
void opcode_1B();
void opcode_1C();
void opcode_1D();
void opcode_1E();
void opcode_1F();
void opcode_20();
void opcode_21();
void opcode_22();
void opcode_23();
void opcode_24();
void opcode_25();
void opcode_26();
void opcode_27();
void opcode_28();
void opcode_29();
void opcode_2A();
void opcode_2B();
void opcode_2C();
void opcode_2D();
void opcode_2E();
void opcode_2F();
void opcode_30();
void opcode_31();
void opcode_32();
void opcode_33();
void opcode_34();
void opcode_35();
void opcode_36();
void opcode_37();
void opcode_38();
void opcode_39();
void opcode_3A();
void opcode_3B();
void opcode_3C();
void opcode_3D();
void opcode_3E();
void opcode_3F();
void opcode_40();
void opcode_41();
void opcode_42();
void opcode_43();
void opcode_44();
void opcode_45();
void opcode_46();
void opcode_47();
void opcode_48();
void opcode_49();
void opcode_4A();
void opcode_4B();
void opcode_4C();
void opcode_4D();
void opcode_4E();
void opcode_4F();
void opcode_50();
void opcode_51();
void opcode_52();
void opcode_53();
void opcode_54();
void opcode_55();
void opcode_56();
void opcode_57();
void opcode_58();
void opcode_59();
void opcode_5A();
void opcode_5B();
void opcode_5C();
void opcode_5D();
void opcode_5E();
void opcode_5F();
void opcode_60();
void opcode_61();
void opcode_62();
void opcode_63();
void opcode_64();
void opcode_65();
void opcode_66();
void opcode_67();
void opcode_68();
void opcode_69();
void opcode_6A();
void opcode_6B();
void opcode_6C();
void opcode_6D();
void opcode_6E();
void opcode_6F();
void opcode_70();
void opcode_71();
void opcode_72();
void opcode_73();
void opcode_74();
void opcode_75();
void opcode_76();
void opcode_77();
void opcode_78();
void opcode_79();
void opcode_7A();
void opcode_7B();
void opcode_7C();
void opcode_7D();
void opcode_7E();
void opcode_7F();
void opcode_80();
void opcode_81();
void opcode_82();
void opcode_83();
void opcode_84();
void opcode_85();
void opcode_86();
void opcode_87();
void opcode_88();
void opcode_89();
void opcode_8A();
void opcode_8B();
void opcode_8C();
void opcode_8D();
void opcode_8E();
void opcode_8F();
void opcode_90();
void opcode_91();
void opcode_92();
void opcode_93();
void opcode_94();
void opcode_95();
void opcode_96();
void opcode_97();
void opcode_98();
void opcode_99();
void opcode_9A();
void opcode_9B();
void opcode_9C();
void opcode_9D();
void opcode_9E();
void opcode_9F();
void opcode_A0();
void opcode_A1();
void opcode_A2();
void opcode_A3();
void opcode_A4();
void opcode_A5();
void opcode_A6();
void opcode_A7();
void opcode_A8();
void opcode_A9();
void opcode_AA();
void opcode_AB();
void opcode_AC();
void opcode_AD();
void opcode_AE();
void opcode_AF();
void opcode_B0();
void opcode_B1();
void opcode_B2();
void opcode_B3();
void opcode_B4();
void opcode_B5();
void opcode_B6();
void opcode_B7();
void opcode_B8();
void opcode_B9();
void opcode_BA();
void opcode_BB();
void opcode_BC();
void opcode_BD();
void opcode_BE();
void opcode_BF();
void opcode_C0();
void opcode_C1();
void opcode_C2();
void opcode_C3();
void opcode_C4();
void opcode_C5();
void opcode_C6();
void opcode_C7();
void opcode_C8();
void opcode_C9();
void opcode_CA();
void opcode_CB();
void opcode_CC();
void opcode_CD();
void opcode_CE();
void opcode_CF();
void opcode_D0();
void opcode_D1();
void opcode_D2();
void opcode_D4();
void opcode_D5();
void opcode_D6();
void opcode_D7();
void opcode_D8();
void opcode_D9();
void opcode_DA();
void opcode_DC();
void opcode_DE();
void opcode_DF();
void opcode_E0();
void opcode_E1();
void opcode_E2();
void opcode_E5();
void opcode_E6();
void opcode_E7();
void opcode_E8();
void opcode_E9();
void opcode_EA();
void opcode_EE();
void opcode_EF();
void opcode_F0();
void opcode_F1();
void opcode_F2();
void opcode_F3();
void opcode_F5();
void opcode_F6();
void opcode_F7();
void opcode_F8();
void opcode_F9();
void opcode_FA();
void opcode_FB();
void opcode_FE();
void opcode_FF();

#endif // OPCODES_H
