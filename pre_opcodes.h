<<<<<<< HEAD
#ifndef PRE_OPCODES_H
#define PRE_OPCODES_H
#include "cpu_types.h"
#include "registers.h"
#include "cpu.h"

static inline void RLC(u_int8 * byte)
{
    u_int8 bit = (*byte >> 7) & 1; // get 7th bit
    *byte <<=  1; // shift left 1
    *byte |= bit;
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_h();
    reset_n();
}

static inline void RRC(u_int8 * byte)
{
    u_int8 bit = (*byte) & 1; // get 0th bit
    *byte >>=  1; // shift right 1
    *byte |= (bit << 7);
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_h();
    reset_n();
}

static inline void RL(u_int8 * byte)
{
    u_int8 bit = (*byte >> 7) & 1; // get 7th bit
    u_int8 carry = (regs.byte.F >> 4) & 1; // get carry bit
    *byte <<= 1; // shift left 1
    *byte |= carry;
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
}

static inline void RR(u_int8 * byte)
{
    u_int8 bit = (*byte) & 1; // get 0th bit
    u_int8 carry = (regs.byte.F >> 4) & 1; // get carry bit
    *byte >>= 1; // shift left 1
    *byte |= (carry << 7);
    if (bit == 1) {
        set_c();
    } else {
        reset_c();
    }
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
}

static inline void SLA(u_int8 * byte)
{
    u_int8 bit = (*byte >> 7) & 1; // get 7th bit
    *byte <<= 1;
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_h();
    reset_n();
}

static inline void SRA(u_int8 * byte)
{
    u_int8 bit = (*byte >> 7) & 1; // get 7th bit
    if (((*byte) & 0x01) == 0x01) {
        set_c();
    } else {
        reset_c();
    }
    *byte >>= 1;
    *byte ^= (-bit ^ *byte) & (1 << 0x07); // copy bit 7 back
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
}

static inline void SWAP(u_int8 * byte)
{
    u_int8 low_nibble = 0x0F & *byte;
    u_int8 high_nibble = 0xF0 & *byte;
    *byte = (high_nibble >> 4) | (low_nibble << 4);
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
    reset_c();
}

static inline void SRL(u_int8 * byte)
{
    u_int8 bit = 0x01 & *byte; // get bit 0
    regs.byte.F ^= (-bit ^ regs.byte.F) & (1 << 0x04); // copy bit 0 to carry bit
    *byte >>= 1;
    if (*byte == 0) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    reset_h();
}

static inline void BIT(u_int8 * byte, int bit)
{
    if ((*byte & (0x01 << bit)) == 0x00) {
        set_z();
    } else {
        reset_z();
    }
    reset_n();
    set_h();
}

void pre_opcode_00();
void pre_opcode_01();
void pre_opcode_02();
void pre_opcode_03();
void pre_opcode_04();
void pre_opcode_05();
void pre_opcode_06();
void pre_opcode_07();
void pre_opcode_08();
void pre_opcode_09();
void pre_opcode_0A();
void pre_opcode_0B();
void pre_opcode_0C();
void pre_opcode_0D();
void pre_opcode_0E();
void pre_opcode_0F();
void pre_opcode_10();
void pre_opcode_11();
void pre_opcode_12();
void pre_opcode_13();
void pre_opcode_14();
void pre_opcode_15();
void pre_opcode_16();
void pre_opcode_17();
void pre_opcode_18();
void pre_opcode_19();
void pre_opcode_1A();
void pre_opcode_1B();
void pre_opcode_1C();
void pre_opcode_1D();
void pre_opcode_1E();
void pre_opcode_1F();
void pre_opcode_20();
void pre_opcode_21();
void pre_opcode_22();
void pre_opcode_23();
void pre_opcode_24();
void pre_opcode_25();
void pre_opcode_26();
void pre_opcode_27();
void pre_opcode_28();
void pre_opcode_29();
void pre_opcode_2A();
void pre_opcode_2B();
void pre_opcode_2C();
void pre_opcode_2D();
void pre_opcode_2E();
void pre_opcode_2F();
void pre_opcode_30();
void pre_opcode_31();
void pre_opcode_32();
void pre_opcode_33();
void pre_opcode_34();
void pre_opcode_35();
void pre_opcode_36();
void pre_opcode_37();
void pre_opcode_38();
void pre_opcode_39();
void pre_opcode_3A();
void pre_opcode_3B();
void pre_opcode_3C();
void pre_opcode_3D();
void pre_opcode_3E();
void pre_opcode_3F();
void pre_opcode_40();
void pre_opcode_41();
void pre_opcode_42();
void pre_opcode_43();
void pre_opcode_44();
void pre_opcode_45();
void pre_opcode_46();
void pre_opcode_47();
void pre_opcode_48();
void pre_opcode_49();
void pre_opcode_4A();
void pre_opcode_4B();
void pre_opcode_4C();
void pre_opcode_4D();
void pre_opcode_4E();
void pre_opcode_4F();
void pre_opcode_50();
void pre_opcode_51();
void pre_opcode_52();
void pre_opcode_53();
void pre_opcode_54();
void pre_opcode_55();
void pre_opcode_56();
void pre_opcode_57();
void pre_opcode_58();
void pre_opcode_59();
void pre_opcode_5A();
void pre_opcode_5B();
void pre_opcode_5C();
void pre_opcode_5D();
void pre_opcode_5E();
void pre_opcode_5F();
void pre_opcode_60();
void pre_opcode_61();
void pre_opcode_62();
void pre_opcode_63();
void pre_opcode_64();
void pre_opcode_65();
void pre_opcode_66();
void pre_opcode_67();
void pre_opcode_68();
void pre_opcode_69();
void pre_opcode_6A();
void pre_opcode_6B();
void pre_opcode_6C();
void pre_opcode_6D();
void pre_opcode_6E();
void pre_opcode_6F();
void pre_opcode_70();
void pre_opcode_71();
void pre_opcode_72();
void pre_opcode_73();
void pre_opcode_74();
void pre_opcode_75();
void pre_opcode_76();
void pre_opcode_77();
void pre_opcode_78();
void pre_opcode_79();
void pre_opcode_7A();
void pre_opcode_7B();
void pre_opcode_7C();
void pre_opcode_7D();
void pre_opcode_7E();
void pre_opcode_7F();
void pre_opcode_80();
void pre_opcode_81();
void pre_opcode_82();
void pre_opcode_83();
void pre_opcode_84();
void pre_opcode_85();
void pre_opcode_86();
void pre_opcode_87();
void pre_opcode_88();
void pre_opcode_89();
void pre_opcode_8A();
void pre_opcode_8B();
void pre_opcode_8C();
void pre_opcode_8D();
void pre_opcode_8E();
void pre_opcode_8F();
void pre_opcode_90();
void pre_opcode_91();
void pre_opcode_92();
void pre_opcode_93();
void pre_opcode_94();
void pre_opcode_95();
void pre_opcode_96();
void pre_opcode_97();
void pre_opcode_98();
void pre_opcode_99();
void pre_opcode_9A();
void pre_opcode_9B();
void pre_opcode_9C();
void pre_opcode_9D();
void pre_opcode_9E();
void pre_opcode_9F();
void pre_opcode_A0();
void pre_opcode_A1();
void pre_opcode_A2();
void pre_opcode_A3();
void pre_opcode_A4();
void pre_opcode_A5();
void pre_opcode_A6();
void pre_opcode_A7();
void pre_opcode_A8();
void pre_opcode_A9();
void pre_opcode_AA();
void pre_opcode_AB();
void pre_opcode_AC();
void pre_opcode_AD();
void pre_opcode_AE();
void pre_opcode_AF();
void pre_opcode_B0();
void pre_opcode_B1();
void pre_opcode_B2();
void pre_opcode_B3();
void pre_opcode_B4();
void pre_opcode_B5();
void pre_opcode_B6();
void pre_opcode_B7();
void pre_opcode_B8();
void pre_opcode_B9();
void pre_opcode_BA();
void pre_opcode_BB();
void pre_opcode_BC();
void pre_opcode_BD();
void pre_opcode_BE();
void pre_opcode_BF();
void pre_opcode_C0();
void pre_opcode_C1();
void pre_opcode_C2();
void pre_opcode_C3();
void pre_opcode_C4();
void pre_opcode_C5();
void pre_opcode_C6();
void pre_opcode_C7();
void pre_opcode_C8();
void pre_opcode_C9();
void pre_opcode_CA();
void pre_opcode_CB();
void pre_opcode_CC();
void pre_opcode_CD();
void pre_opcode_CE();
void pre_opcode_CF();
void pre_opcode_D0();
void pre_opcode_D1();
void pre_opcode_D2();
void pre_opcode_D3();
void pre_opcode_D4();
void pre_opcode_D5();
void pre_opcode_D6();
void pre_opcode_D7();
void pre_opcode_D8();
void pre_opcode_D9();
void pre_opcode_DA();
void pre_opcode_DB();
void pre_opcode_DC();
void pre_opcode_DD();
void pre_opcode_DE();
void pre_opcode_DF();
void pre_opcode_E0();
void pre_opcode_E1();
void pre_opcode_E2();
void pre_opcode_E3();
void pre_opcode_E4();
void pre_opcode_E5();
void pre_opcode_E6();
void pre_opcode_E7();
void pre_opcode_E8();
void pre_opcode_E9();
void pre_opcode_EA();
void pre_opcode_EB();
void pre_opcode_EC();
void pre_opcode_ED();
void pre_opcode_EE();
void pre_opcode_EF();
void pre_opcode_F0();
void pre_opcode_F1();
void pre_opcode_F2();
void pre_opcode_F3();
void pre_opcode_F4();
void pre_opcode_F5();
void pre_opcode_F6();
void pre_opcode_F7();
void pre_opcode_F8();
void pre_opcode_F9();
void pre_opcode_FA();
void pre_opcode_FB();
void pre_opcode_FC();
void pre_opcode_FD();
void pre_opcode_FE();
void pre_opcode_FF();

#endif // PRE_OPCODES_H
=======
#ifndef PRE_OPCODES_H
#define PRE_OPCODES_H

void pre_opcode_00();
void pre_opcode_01();
void pre_opcode_02();
void pre_opcode_03();
void pre_opcode_04();
void pre_opcode_05();
void pre_opcode_06();
void pre_opcode_07();
void pre_opcode_08();
void pre_opcode_09();
void pre_opcode_0A();
void pre_opcode_0B();
void pre_opcode_0C();
void pre_opcode_0D();
void pre_opcode_0E();
void pre_opcode_0F();
void pre_opcode_10();
void pre_opcode_11();
void pre_opcode_12();
void pre_opcode_13();
void pre_opcode_14();
void pre_opcode_15();
void pre_opcode_16();
void pre_opcode_17();
void pre_opcode_18();
void pre_opcode_19();
void pre_opcode_1A();
void pre_opcode_1B();
void pre_opcode_1C();
void pre_opcode_1D();
void pre_opcode_1E();
void pre_opcode_1F();
void pre_opcode_20();
void pre_opcode_21();
void pre_opcode_22();
void pre_opcode_23();
void pre_opcode_24();
void pre_opcode_25();
void pre_opcode_26();
void pre_opcode_27();
void pre_opcode_28();
void pre_opcode_29();
void pre_opcode_2A();
void pre_opcode_2B();
void pre_opcode_2C();
void pre_opcode_2D();
void pre_opcode_2E();
void pre_opcode_2F();
void pre_opcode_30();
void pre_opcode_31();
void pre_opcode_32();
void pre_opcode_33();
void pre_opcode_34();
void pre_opcode_35();
void pre_opcode_36();
void pre_opcode_37();
void pre_opcode_38();
void pre_opcode_39();
void pre_opcode_3A();
void pre_opcode_3B();
void pre_opcode_3C();
void pre_opcode_3D();
void pre_opcode_3E();
void pre_opcode_3F();
void pre_opcode_40();
void pre_opcode_41();
void pre_opcode_42();
void pre_opcode_43();
void pre_opcode_44();
void pre_opcode_45();
void pre_opcode_46();
void pre_opcode_47();
void pre_opcode_48();
void pre_opcode_49();
void pre_opcode_4A();
void pre_opcode_4B();
void pre_opcode_4C();
void pre_opcode_4D();
void pre_opcode_4E();
void pre_opcode_4F();
void pre_opcode_50();
void pre_opcode_51();
void pre_opcode_52();
void pre_opcode_53();
void pre_opcode_54();
void pre_opcode_55();
void pre_opcode_56();
void pre_opcode_57();
void pre_opcode_58();
void pre_opcode_59();
void pre_opcode_5A();
void pre_opcode_5B();
void pre_opcode_5C();
void pre_opcode_5D();
void pre_opcode_5E();
void pre_opcode_5F();
void pre_opcode_60();
void pre_opcode_61();
void pre_opcode_62();
void pre_opcode_63();
void pre_opcode_64();
void pre_opcode_65();
void pre_opcode_66();
void pre_opcode_67();
void pre_opcode_68();
void pre_opcode_69();
void pre_opcode_6A();
void pre_opcode_6B();
void pre_opcode_6C();
void pre_opcode_6D();
void pre_opcode_6E();
void pre_opcode_6F();
void pre_opcode_70();
void pre_opcode_71();
void pre_opcode_72();
void pre_opcode_73();
void pre_opcode_74();
void pre_opcode_75();
void pre_opcode_76();
void pre_opcode_77();
void pre_opcode_78();
void pre_opcode_79();
void pre_opcode_7A();
void pre_opcode_7B();
void pre_opcode_7C();
void pre_opcode_7D();
void pre_opcode_7E();
void pre_opcode_7F();
void pre_opcode_80();
void pre_opcode_81();
void pre_opcode_82();
void pre_opcode_83();
void pre_opcode_84();
void pre_opcode_85();
void pre_opcode_86();
void pre_opcode_87();
void pre_opcode_88();
void pre_opcode_89();
void pre_opcode_8A();
void pre_opcode_8B();
void pre_opcode_8C();
void pre_opcode_8D();
void pre_opcode_8E();
void pre_opcode_8F();
void pre_opcode_90();
void pre_opcode_91();
void pre_opcode_92();
void pre_opcode_93();
void pre_opcode_94();
void pre_opcode_95();
void pre_opcode_96();
void pre_opcode_97();
void pre_opcode_98();
void pre_opcode_99();
void pre_opcode_9A();
void pre_opcode_9B();
void pre_opcode_9C();
void pre_opcode_9D();
void pre_opcode_9E();
void pre_opcode_9F();
void pre_opcode_A0();
void pre_opcode_A1();
void pre_opcode_A2();
void pre_opcode_A3();
void pre_opcode_A4();
void pre_opcode_A5();
void pre_opcode_A6();
void pre_opcode_A7();
void pre_opcode_A8();
void pre_opcode_A9();
void pre_opcode_AA();
void pre_opcode_AB();
void pre_opcode_AC();
void pre_opcode_AD();
void pre_opcode_AE();
void pre_opcode_AF();
void pre_opcode_B0();
void pre_opcode_B1();
void pre_opcode_B2();
void pre_opcode_B3();
void pre_opcode_B4();
void pre_opcode_B5();
void pre_opcode_B6();
void pre_opcode_B7();
void pre_opcode_B8();
void pre_opcode_B9();
void pre_opcode_BA();
void pre_opcode_BB();
void pre_opcode_BC();
void pre_opcode_BD();
void pre_opcode_BE();
void pre_opcode_BF();
void pre_opcode_C0();
void pre_opcode_C1();
void pre_opcode_C2();
void pre_opcode_C3();
void pre_opcode_C4();
void pre_opcode_C5();
void pre_opcode_C6();
void pre_opcode_C7();
void pre_opcode_C8();
void pre_opcode_C9();
void pre_opcode_CA();
void pre_opcode_CB();
void pre_opcode_CC();
void pre_opcode_CD();
void pre_opcode_CE();
void pre_opcode_CF();
void pre_opcode_D0();
void pre_opcode_D1();
void pre_opcode_D2();
void pre_opcode_D3();
void pre_opcode_D4();
void pre_opcode_D5();
void pre_opcode_D6();
void pre_opcode_D7();
void pre_opcode_D8();
void pre_opcode_D9();
void pre_opcode_DA();
void pre_opcode_DB();
void pre_opcode_DC();
void pre_opcode_DD();
void pre_opcode_DE();
void pre_opcode_DF();
void pre_opcode_E0();
void pre_opcode_E1();
void pre_opcode_E2();
void pre_opcode_E3();
void pre_opcode_E4();
void pre_opcode_E5();
void pre_opcode_E6();
void pre_opcode_E7();
void pre_opcode_E8();
void pre_opcode_E9();
void pre_opcode_EA();
void pre_opcode_EB();
void pre_opcode_EC();
void pre_opcode_ED();
void pre_opcode_EE();
void pre_opcode_EF();
void pre_opcode_F0();
void pre_opcode_F1();
void pre_opcode_F2();
void pre_opcode_F3();
void pre_opcode_F4();
void pre_opcode_F5();
void pre_opcode_F6();
void pre_opcode_F7();
void pre_opcode_F8();
void pre_opcode_F9();
void pre_opcode_FA();
void pre_opcode_FB();
void pre_opcode_FC();
void pre_opcode_FD();
void pre_opcode_FE();
void pre_opcode_FF();

#endif // PRE_OPCODES_H
>>>>>>> origin/master
