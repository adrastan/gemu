#include "pre_opcode.h"

PreOpcode::PreOpcode(Memory *memory, Cpu *cpu)
{
    this->memory = memory;
    this->cpu = cpu;
}

void PreOpcode::RLC(u8 * byte)
{
    u8 bit = (*byte >> 7) & 1; // get 7th bit
    *byte <<=  1; // shift left 1
    *byte |= bit;
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_h();
    this->cpu->reset_n();
}

void PreOpcode::RRC(u8 * byte)
{
    u8 bit = (*byte) & 1; // get 0th bit
    *byte >>=  1; // shift right 1
    *byte |= (bit << 7);
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_h();
    this->cpu->reset_n();
}

void PreOpcode::RL(u8 * byte)
{
    u8 bit = (*byte >> 7) & 1; // get 7th bit
    u8 carry = (this->cpu->regs.byte.F >> 4) & 1; // get carry bit
    *byte <<= 1; // shift left 1
    *byte |= carry;
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_n();
    this->cpu->reset_h();
}

void PreOpcode::RR(u8 * byte)
{
    u8 bit = (*byte) & 1; // get 0th bit
    u8 carry = (this->cpu->regs.byte.F >> 4) & 1; // get carry bit
    *byte >>= 1; // shift right 1
    *byte |= (carry << 7);
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_n();
    this->cpu->reset_h();
}

void PreOpcode::SLA(u8 * byte)
{
    u8 bit = (*byte >> 7) & 1; // get 7th bit
    *byte <<= 1;
    this->cpu->regs.byte.F ^= (-bit ^ this->cpu->regs.byte.F) & (1 << 0x04); // copy bit 7 to carry bit
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_h();
    this->cpu->reset_n();
}

void PreOpcode::SRA(u8 * byte)
{
    u8 bit = (*byte >> 7) & 1; // get 7th bit
    if (((*byte) & 0x01) == 0x01) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    *byte >>= 1;
    *byte ^= (-bit ^ *byte) & (1 << 0x07); // copy bit 7 back
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_n();
    this->cpu->reset_h();
}

void PreOpcode::SWAP(u8 * byte)
{
    u8 low_nibble = 0x0F & *byte;
    u8 high_nibble = 0xF0 & *byte;
    *byte = (high_nibble >> 4) | (low_nibble << 4);
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_n();
    this->cpu->reset_h();
    this->cpu->reset_c();
}

void PreOpcode::SRL(u8 * byte)
{
    if (*byte & 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    *byte >>= 1;
    if (*byte == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_n();
    this->cpu->reset_h();
}

void PreOpcode::BIT(u8 * byte, int bit)
{
    if ((*byte & (0x01 << bit)) == 0x00) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
    this->cpu->reset_n();
    this->cpu->set_h();
}

// RLC B
void PreOpcode::pre_opcode_00()
{
    RLC(&this->cpu->regs.byte.B);
}

// RLC C
void PreOpcode::pre_opcode_01()
{
    RLC(&this->cpu->regs.byte.C);
}

// RLC D
void PreOpcode::pre_opcode_02()
{
    RLC(&this->cpu->regs.byte.D);
}

// RLC E
void PreOpcode::pre_opcode_03()
{
    RLC(&this->cpu->regs.byte.E);
}

// RLC H
void PreOpcode::pre_opcode_04()
{
    RLC(&this->cpu->regs.byte.H);
}

// RLC L
void PreOpcode::pre_opcode_05()
{
    RLC(&this->cpu->regs.byte.L);
}

// RLC (HL)
void PreOpcode::pre_opcode_06()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    RLC(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// RLC A
void PreOpcode::pre_opcode_07()
{
    RLC(&this->cpu->regs.byte.A);
}

// RRC B
void PreOpcode::pre_opcode_08()
{
    RRC(&this->cpu->regs.byte.B);
}

// RRC C
void PreOpcode::pre_opcode_09()
{
    RRC(&this->cpu->regs.byte.C);
}

// RRC D
void PreOpcode::pre_opcode_0A()
{
    RRC(&this->cpu->regs.byte.D);
}

// RRC E
void PreOpcode::pre_opcode_0B()
{
    RRC(&this->cpu->regs.byte.E);
}

// RRC H
void PreOpcode::pre_opcode_0C()
{
    RRC(&this->cpu->regs.byte.H);
}

// RRC L
void PreOpcode::pre_opcode_0D()
{
    RRC(&this->cpu->regs.byte.L);
}

// RRC (HL)
void PreOpcode::pre_opcode_0E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    RRC(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// RRC A
void PreOpcode::pre_opcode_0F()
{
    RRC(&this->cpu->regs.byte.A);
}

// RL B
void PreOpcode::pre_opcode_10()
{
    RL(&this->cpu->regs.byte.B);
}

// RL C
void PreOpcode::pre_opcode_11()
{
    RL(&this->cpu->regs.byte.C);
}

// RL D
void PreOpcode::pre_opcode_12()
{
    RL(&this->cpu->regs.byte.D);
}

// RL E
void PreOpcode::pre_opcode_13()
{
    RL(&this->cpu->regs.byte.E);
}

// RL H
void PreOpcode::pre_opcode_14()
{
    RL(&this->cpu->regs.byte.H);
}

// RL L
void PreOpcode::pre_opcode_15()
{
    RL(&this->cpu->regs.byte.L);
}

// RL (HL)
void PreOpcode::pre_opcode_16()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    RL(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// RL A
void PreOpcode::pre_opcode_17()
{
    RL(&this->cpu->regs.byte.A);
}

// RR B
void PreOpcode::pre_opcode_18()
{
    RR(&this->cpu->regs.byte.B);
}

// RR C
void PreOpcode::pre_opcode_19()
{
    RR(&this->cpu->regs.byte.C);
}

// RR D
void PreOpcode::pre_opcode_1A()
{
    RR(&this->cpu->regs.byte.D);
}

// RR E
void PreOpcode::pre_opcode_1B()
{
    RR(&this->cpu->regs.byte.E);
}

// RR H
void PreOpcode::pre_opcode_1C()
{
    RR(&this->cpu->regs.byte.H);
}

// RR L
void PreOpcode::pre_opcode_1D()
{
    RR(&this->cpu->regs.byte.L);
}

// RR (HL)
void PreOpcode::pre_opcode_1E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    RR(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// RR A
void PreOpcode::pre_opcode_1F()
{
    RR(&this->cpu->regs.byte.A);
}

// SLA B
void PreOpcode::pre_opcode_20()
{
    SLA(&this->cpu->regs.byte.B);
}

// SLA C
void PreOpcode::pre_opcode_21()
{
    SLA(&this->cpu->regs.byte.C);
}

// SLA D
void PreOpcode::pre_opcode_22()
{
    SLA(&this->cpu->regs.byte.D);
}

// SLA E
void PreOpcode::pre_opcode_23()
{
    SLA(&this->cpu->regs.byte.E);
}

// SLA H
void PreOpcode::pre_opcode_24()
{
    SLA(&this->cpu->regs.byte.H);
}

// SLA L
void PreOpcode::pre_opcode_25()
{
    SLA(&this->cpu->regs.byte.L);
}

// SLA (HL)
void PreOpcode::pre_opcode_26()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    SLA(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// SLA A
void PreOpcode::pre_opcode_27()
{
    SLA(&this->cpu->regs.byte.A);
}

// SRA B
void PreOpcode::pre_opcode_28()
{
    SRA(&this->cpu->regs.byte.B);
}

// SRA C
void PreOpcode::pre_opcode_29()
{
    SRA(&this->cpu->regs.byte.C);
}

// SRA D
void PreOpcode::pre_opcode_2A()
{
    SRA(&this->cpu->regs.byte.D);
}

// SRA E
void PreOpcode::pre_opcode_2B()
{
    SRA(&this->cpu->regs.byte.E);
}

// SRA H
void PreOpcode::pre_opcode_2C()
{
    SRA(&this->cpu->regs.byte.H);
}

// SRA L
void PreOpcode::pre_opcode_2D()
{
    SRA(&this->cpu->regs.byte.L);
}

// SRA (HL)
void PreOpcode::pre_opcode_2E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    SRA(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// SRA A
void PreOpcode::pre_opcode_2F()
{
    SRA(&this->cpu->regs.byte.A);
}

// SWAP B
void PreOpcode::pre_opcode_30()
{
    SWAP(&this->cpu->regs.byte.B);
}

// SWAP C
void PreOpcode::pre_opcode_31()
{
    SWAP(&this->cpu->regs.byte.C);
}

// SWAP D
void PreOpcode::pre_opcode_32()
{
    SWAP(&this->cpu->regs.byte.D);
}

// SWAP E
void PreOpcode::pre_opcode_33()
{
    SWAP(&this->cpu->regs.byte.E);
}

// SWAP H
void PreOpcode::pre_opcode_34()
{
    SWAP(&this->cpu->regs.byte.H);
}

// SWAP L
void PreOpcode::pre_opcode_35()
{
    SWAP(&this->cpu->regs.byte.L);
}

// SWAP (HL)
void PreOpcode::pre_opcode_36()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    SWAP(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// SWAP A
void PreOpcode::pre_opcode_37()
{
    SWAP(&this->cpu->regs.byte.A);
}

// SRL B
void PreOpcode::pre_opcode_38()
{
    SRL(&this->cpu->regs.byte.B);
}

// SRL C
void PreOpcode::pre_opcode_39()
{
    SRL(&this->cpu->regs.byte.C);
}

// SRL D
void PreOpcode::pre_opcode_3A()
{
    SRL(&this->cpu->regs.byte.D);
}

// SRL E
void PreOpcode::pre_opcode_3B()
{
    SRL(&this->cpu->regs.byte.E);
}

// SRL H
void PreOpcode::pre_opcode_3C()
{
    SRL(&this->cpu->regs.byte.H);
}

// SRL L
void PreOpcode::pre_opcode_3D()
{
    SRL(&this->cpu->regs.byte.L);
}

// SRL (HL)
void PreOpcode::pre_opcode_3E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    SRL(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// SRL A
void PreOpcode::pre_opcode_3F()
{
    SRL(&this->cpu->regs.byte.A);
}

// BIT 0,B
void PreOpcode::pre_opcode_40()
{
    BIT(&this->cpu->regs.byte.B,0);
}

// BIT 0,C
void PreOpcode::pre_opcode_41()
{
    BIT(&this->cpu->regs.byte.C,0);
}

// BIT 0,D
void PreOpcode::pre_opcode_42()
{
    BIT(&this->cpu->regs.byte.D,0);
}

// BIT 0,E
void PreOpcode::pre_opcode_43()
{
    BIT(&this->cpu->regs.byte.E,0);
}

// BIT 0,H
void PreOpcode::pre_opcode_44()
{
    BIT(&this->cpu->regs.byte.H,0);
}

// BIT 0,L
void PreOpcode::pre_opcode_45()
{
    BIT(&this->cpu->regs.byte.L,0);
}

// BIT 0,(HL)
void PreOpcode::pre_opcode_46()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 0);
}

// BIT 0,A
void PreOpcode::pre_opcode_47()
{
    BIT(&this->cpu->regs.byte.A,0);
}

// BIT 1,B
void PreOpcode::pre_opcode_48()
{
    BIT(&this->cpu->regs.byte.B,1);
}

// BIT 1,C
void PreOpcode::pre_opcode_49()
{
    BIT(&this->cpu->regs.byte.C,1);
}

// BIT 1,D
void PreOpcode::pre_opcode_4A()
{
    BIT(&this->cpu->regs.byte.D,1);
}

// BIT 1,E
void PreOpcode::pre_opcode_4B()
{
    BIT(&this->cpu->regs.byte.E,1);
}

// BIT 1,H
void PreOpcode::pre_opcode_4C()
{
    BIT(&this->cpu->regs.byte.H,1);
}

// BIT 1,L
void PreOpcode::pre_opcode_4D()
{
    BIT(&this->cpu->regs.byte.L,1);
}

// BIT 1,(HL)
void PreOpcode::pre_opcode_4E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 1);
}

// BIT 1,A
void PreOpcode::pre_opcode_4F()
{
    BIT(&this->cpu->regs.byte.A,1);
}

// BIT 2,B
void PreOpcode::pre_opcode_50()
{
    BIT(&this->cpu->regs.byte.B,2);
}

// BIT 2,C
void PreOpcode::pre_opcode_51()
{
    BIT(&this->cpu->regs.byte.C,2);
}

// BIT 2,D
void PreOpcode::pre_opcode_52()
{
    BIT(&this->cpu->regs.byte.D,2);
}

// BIT 2,E
void PreOpcode::pre_opcode_53()
{
    BIT(&this->cpu->regs.byte.E,2);
}

// BIT 2,H
void PreOpcode::pre_opcode_54()
{
    BIT(&this->cpu->regs.byte.H,2);
}

// BIT 2,L
void PreOpcode::pre_opcode_55()
{
    BIT(&this->cpu->regs.byte.L,2);
}

// BIT 2,(HL)
void PreOpcode::pre_opcode_56()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 2);
}

// BIT 2,A
void PreOpcode::pre_opcode_57()
{
    BIT(&this->cpu->regs.byte.A,2);
}

// BIT 3,B
void PreOpcode::pre_opcode_58()
{
    BIT(&this->cpu->regs.byte.B,3);
}

// BIT 3,C
void PreOpcode::pre_opcode_59()
{
    BIT(&this->cpu->regs.byte.C,3);
}

// BIT 3,D
void PreOpcode::pre_opcode_5A()
{
    BIT(&this->cpu->regs.byte.D,3);
}

// BIT 3,E
void PreOpcode::pre_opcode_5B()
{
    BIT(&this->cpu->regs.byte.E,3);
}

// BIT 3,H
void PreOpcode::pre_opcode_5C()
{
    BIT(&this->cpu->regs.byte.H,3);
}

// BIT 3,L
void PreOpcode::pre_opcode_5D()
{
    BIT(&this->cpu->regs.byte.L,3);
}

// BIT 3,(HL)
void PreOpcode::pre_opcode_5E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 3);
}

// BIT 3,A
void PreOpcode::pre_opcode_5F()
{
    BIT(&this->cpu->regs.byte.A,3);
}

// BIT 4,B
void PreOpcode::pre_opcode_60()
{
    BIT(&this->cpu->regs.byte.B,4);
}

// BIT 4,C
void PreOpcode::pre_opcode_61()
{
    BIT(&this->cpu->regs.byte.C,4);
}

// BIT 4,D
void PreOpcode::pre_opcode_62()
{
    BIT(&this->cpu->regs.byte.D,4);
}

// BIT 4,E
void PreOpcode::pre_opcode_63()
{
    BIT(&this->cpu->regs.byte.E,4);
}

// BIT 4,H
void PreOpcode::pre_opcode_64()
{
    BIT(&this->cpu->regs.byte.H,4);
}

// BIT 4,L
void PreOpcode::pre_opcode_65()
{
    BIT(&this->cpu->regs.byte.L,4);
}

// BIT 4,(HL)
void PreOpcode::pre_opcode_66()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 4);
}

// BIT 4,A
void PreOpcode::pre_opcode_67()
{
    BIT(&this->cpu->regs.byte.A,4);
}

// BIT 5,B
void PreOpcode::pre_opcode_68()
{
    BIT(&this->cpu->regs.byte.B,5);
}

// BIT 5,C
void PreOpcode::pre_opcode_69()
{
    BIT(&this->cpu->regs.byte.C,5);
}

// BIT 5,D
void PreOpcode::pre_opcode_6A()
{
    BIT(&this->cpu->regs.byte.D,5);
}

// BIT 5,E
void PreOpcode::pre_opcode_6B()
{
    BIT(&this->cpu->regs.byte.E,5);
}

// BIT 5,H
void PreOpcode::pre_opcode_6C()
{
    BIT(&this->cpu->regs.byte.H,5);
}

// BIT 5,L
void PreOpcode::pre_opcode_6D()
{
    BIT(&this->cpu->regs.byte.L,5);
}

// BIT 5,(HL)
void PreOpcode::pre_opcode_6E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 5);
}

// BIT 5,A
void PreOpcode::pre_opcode_6F()
{
    BIT(&this->cpu->regs.byte.A,5);
}

// BIT 6,B
void PreOpcode::pre_opcode_70()
{
    BIT(&this->cpu->regs.byte.B,6);
}

// BIT 6,C
void PreOpcode::pre_opcode_71()
{
    BIT(&this->cpu->regs.byte.C,6);
}

// BIT 6,D
void PreOpcode::pre_opcode_72()
{
    BIT(&this->cpu->regs.byte.D,6);
}

// BIT 6,E
void PreOpcode::pre_opcode_73()
{
    BIT(&this->cpu->regs.byte.E,6);
}

// BIT 6,H
void PreOpcode::pre_opcode_74()
{
    BIT(&this->cpu->regs.byte.H,6);
}

// BIT 6,L
void PreOpcode::pre_opcode_75()
{
    BIT(&this->cpu->regs.byte.L,6);
}

// BIT 6,(HL)
void PreOpcode::pre_opcode_76()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 6);
}

// BIT 6,A
void PreOpcode::pre_opcode_77()
{
    BIT(&this->cpu->regs.byte.A,6);
}

// BIT 7,B
void PreOpcode::pre_opcode_78()
{
    BIT(&this->cpu->regs.byte.B,7);
}

// BIT 7,C
void PreOpcode::pre_opcode_79()
{
    BIT(&this->cpu->regs.byte.C,7);
}

// BIT 7,D
void PreOpcode::pre_opcode_7A()
{
    BIT(&this->cpu->regs.byte.D,7);
}

// BIT 7,E
void PreOpcode::pre_opcode_7B()
{
    BIT(&this->cpu->regs.byte.E,7);
}

// BIT 7,H
void PreOpcode::pre_opcode_7C()
{
    BIT(&this->cpu->regs.byte.H,7);
}

// BIT 7,L
void PreOpcode::pre_opcode_7D()
{
    BIT(&this->cpu->regs.byte.L,7);
}

// BIT 7,(HL)
void PreOpcode::pre_opcode_7E()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    BIT(&byte, 7);
}

// BIT 7,A
void PreOpcode::pre_opcode_7F()
{
    BIT(&this->cpu->regs.byte.A,7);
}

// RES 0,B
void PreOpcode::pre_opcode_80()
{
    this->cpu->regs.byte.B &= ~(0x01 << 0);
}

// RES 0,C
void PreOpcode::pre_opcode_81()
{
    this->cpu->regs.byte.C &= ~(0x01 << 0);
}

// RES 0,D
void PreOpcode::pre_opcode_82()
{
    this->cpu->regs.byte.D &= ~(0x01 << 0);
}

// RES 0,E
void PreOpcode::pre_opcode_83()
{
    this->cpu->regs.byte.E &= ~(0x01 << 0);
}

// RES 0,H
void PreOpcode::pre_opcode_84()
{
    this->cpu->regs.byte.H &= ~(0x01 << 0);
}

// RES 0,L
void PreOpcode::pre_opcode_85()
{
    this->cpu->regs.byte.L &= ~(0x01 << 0);
}

// RES 0,(HL)
void PreOpcode::pre_opcode_86()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 0)));
}

// RES 0,A
void PreOpcode::pre_opcode_87()
{
    this->cpu->regs.byte.A &= ~(0x01 << 0);
}

// RES 1,B
void PreOpcode::pre_opcode_88()
{
    this->cpu->regs.byte.B &= ~(0x01 << 1);
}

// RES 1,C
void PreOpcode::pre_opcode_89()
{
    this->cpu->regs.byte.C &= ~(0x01 << 1);
}

// RES 1,D
void PreOpcode::pre_opcode_8A()
{
    this->cpu->regs.byte.D &= ~(0x01 << 1);
}

// RES 1,E
void PreOpcode::pre_opcode_8B()
{
    this->cpu->regs.byte.E &= ~(0x01 << 1);
}

// RES 1,H
void PreOpcode::pre_opcode_8C()
{
    this->cpu->regs.byte.H &= ~(0x01 << 1);
}

// RES 1,L
void PreOpcode::pre_opcode_8D()
{
    this->cpu->regs.byte.L &= ~(0x01 << 1);
}

// RES 1,(HL)
void PreOpcode::pre_opcode_8E()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 1)));
}

// RES 1,A
void PreOpcode::pre_opcode_8F()
{
    this->cpu->regs.byte.A &= ~(0x01 << 1);
}

// RES 2,B
void PreOpcode::pre_opcode_90()
{
    this->cpu->regs.byte.B &= ~(0x01 << 2);
}

// RES 2,C
void PreOpcode::pre_opcode_91()
{
    this->cpu->regs.byte.C &= ~(0x01 << 2);
}

// RES 2,D
void PreOpcode::pre_opcode_92()
{
    this->cpu->regs.byte.D &= ~(0x01 << 2);
}

// RES 2,E
void PreOpcode::pre_opcode_93()
{
    this->cpu->regs.byte.E &= ~(0x01 << 2);
}

// RES 2,H
void PreOpcode::pre_opcode_94()
{
    this->cpu->regs.byte.H &= ~(0x01 << 2);
}

// RES 2,L
void PreOpcode::pre_opcode_95()
{
    this->cpu->regs.byte.L &= ~(0x01 << 2);
}

// RES 2,(HL)
void PreOpcode::pre_opcode_96()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 2)));
}

// RES 2,A
void PreOpcode::pre_opcode_97()
{
    this->cpu->regs.byte.A &= ~(0x01 << 2);
}

// RES 3,B
void PreOpcode::pre_opcode_98()
{
    this->cpu->regs.byte.B &= ~(0x01 << 3);
}

// RES 3,C
void PreOpcode::pre_opcode_99()
{
    this->cpu->regs.byte.C &= ~(0x01 << 3);
}

// RES 3,D
void PreOpcode::pre_opcode_9A()
{
    this->cpu->regs.byte.D &= ~(0x01 << 3);
}

// RES 3,E
void PreOpcode::pre_opcode_9B()
{
    this->cpu->regs.byte.E &= ~(0x01 << 3);
}

// RES 3,H
void PreOpcode::pre_opcode_9C()
{
    this->cpu->regs.byte.H &= ~(0x01 << 3);
}

// RES 3,L
void PreOpcode::pre_opcode_9D()
{
    this->cpu->regs.byte.L &= ~(0x01 << 3);
}

// RES 3,(HL)
void PreOpcode::pre_opcode_9E()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 3)));
}

// RES 3,A
void PreOpcode::pre_opcode_9F()
{
    this->cpu->regs.byte.A &= ~(0x01 << 3);
}

// RES 4,B
void PreOpcode::pre_opcode_A0()
{
    this->cpu->regs.byte.B &= ~(0x01 << 4);
}

// RES 4,C
void PreOpcode::pre_opcode_A1()
{
    this->cpu->regs.byte.C &= ~(0x01 << 4);
}

// RES 4,D
void PreOpcode::pre_opcode_A2()
{
    this->cpu->regs.byte.D &= ~(0x01 << 4);
}

// RES 4,E
void PreOpcode::pre_opcode_A3()
{
    this->cpu->regs.byte.E &= ~(0x01 << 4);
}

// RES 4,H
void PreOpcode::pre_opcode_A4()
{
    this->cpu->regs.byte.H &= ~(0x01 << 4);
}

// RES 4,L
void PreOpcode::pre_opcode_A5()
{
    this->cpu->regs.byte.L &= ~(0x01 << 4);
}

// RES 4,(HL)
void PreOpcode::pre_opcode_A6()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 4)));
}

// RES 4,A
void PreOpcode::pre_opcode_A7()
{
    this->cpu->regs.byte.A &= ~(0x01 << 4);
}

// RES 5,B
void PreOpcode::pre_opcode_A8()
{
    this->cpu->regs.byte.B &= ~(0x01 << 5);
}

// RES 5,C
void PreOpcode::pre_opcode_A9()
{
    this->cpu->regs.byte.C &= ~(0x01 << 5);
}

// RES 5,D
void PreOpcode::pre_opcode_AA()
{
    this->cpu->regs.byte.D &= ~(0x01 << 5);
}

// RES 5,E
void PreOpcode::pre_opcode_AB()
{
    this->cpu->regs.byte.E &= ~(0x01 << 5);
}

// RES 5,H
void PreOpcode::pre_opcode_AC()
{
    this->cpu->regs.byte.H &= ~(0x01 << 5);
}

// RES 5,L
void PreOpcode::pre_opcode_AD()
{
    this->cpu->regs.byte.L &= ~(0x01 << 5);
}

// RES 5,(HL)
void PreOpcode::pre_opcode_AE()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 5)));
}

// RES 5,A
void PreOpcode::pre_opcode_AF()
{
    this->cpu->regs.byte.A &= ~(0x01 << 5);
}

// RES 6,B
void PreOpcode::pre_opcode_B0()
{
    this->cpu->regs.byte.B &= ~(0x01 << 6);
}

// RES 6,C
void PreOpcode::pre_opcode_B1()
{
    this->cpu->regs.byte.C &= ~(0x01 << 6);
}

// RES 6,D
void PreOpcode::pre_opcode_B2()
{
    this->cpu->regs.byte.D &= ~(0x01 << 6);
}

// RES 6,E
void PreOpcode::pre_opcode_B3()
{
    this->cpu->regs.byte.E &= ~(0x01 << 6);
}

// RES 6,H
void PreOpcode::pre_opcode_B4()
{
    this->cpu->regs.byte.H &= ~(0x01 << 6);
}

// RES 6,L
void PreOpcode::pre_opcode_B5()
{
    this->cpu->regs.byte.L &= ~(0x01 << 6);
}

// RES 6,(HL)
void PreOpcode::pre_opcode_B6()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 6)));
}

// RES 6,A
void PreOpcode::pre_opcode_B7()
{
    this->cpu->regs.byte.A &= ~(0x01 << 6);
}

// RES 7,B
void PreOpcode::pre_opcode_B8()
{
    this->cpu->regs.byte.B &= ~(0x01 << 7);
}

// RES 7,C
void PreOpcode::pre_opcode_B9()
{
    this->cpu->regs.byte.C &= ~(0x01 << 7);
}

// RES 7,D
void PreOpcode::pre_opcode_BA()
{
    this->cpu->regs.byte.D &= ~(0x01 << 7);
}

// RES 7,E
void PreOpcode::pre_opcode_BB()
{
    this->cpu->regs.byte.E &= ~(0x01 << 7);
}

// RES 7,H
void PreOpcode::pre_opcode_BC()
{
    this->cpu->regs.byte.H &= ~(0x01 << 7);
}

// RES 7,L
void PreOpcode::pre_opcode_BD()
{
    this->cpu->regs.byte.L &= ~(0x01 << 7);
}

// RES 7,(HL)
void PreOpcode::pre_opcode_BE()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) & ~(0x01 << 7)));
}

// RES 7,A
void PreOpcode::pre_opcode_BF()
{
    this->cpu->regs.byte.A &= ~(0x01 << 7);
}

// SET 0,B
void PreOpcode::pre_opcode_C0()
{
    this->cpu->regs.byte.B |= (0x01 << 0);
}

// SET 0,C
void PreOpcode::pre_opcode_C1()
{
    this->cpu->regs.byte.C |= (0x01 << 0);
}

// SET 0,D
void PreOpcode::pre_opcode_C2()
{
    this->cpu->regs.byte.D |= (0x01 << 0);
}

// SET 0,E
void PreOpcode::pre_opcode_C3()
{
    this->cpu->regs.byte.E |= (0x01 << 0);
}

// SET 0,H
void PreOpcode::pre_opcode_C4()
{
    this->cpu->regs.byte.H |= (0x01 << 0);
}

// SET 0,L
void PreOpcode::pre_opcode_C5()
{
    this->cpu->regs.byte.L |= (0x01 << 0);
}

// SET 0,(HL)
void PreOpcode::pre_opcode_C6()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 0)));
}

// SET 0,A
void PreOpcode::pre_opcode_C7()
{
    this->cpu->regs.byte.A |= (0x01 << 0);
}

// SET 1,B
void PreOpcode::pre_opcode_C8()
{
    this->cpu->regs.byte.B |= (0x01 << 1);
}

// SET 1,C
void PreOpcode::pre_opcode_C9()
{
    this->cpu->regs.byte.C |= (0x01 << 1);
}

// SET 1,D
void PreOpcode::pre_opcode_CA()
{
    this->cpu->regs.byte.D |= (0x01 << 1);
}

// SET 1,E
void PreOpcode::pre_opcode_CB()
{
    this->cpu->regs.byte.E |= (0x01 << 1);
}

// SET 1,H
void PreOpcode::pre_opcode_CC()
{
    this->cpu->regs.byte.H |= (0x01 << 1);
}

// SET 1,L
void PreOpcode::pre_opcode_CD()
{
    this->cpu->regs.byte.L |= (0x01 << 1);
}

// SET 1,(HL)
void PreOpcode::pre_opcode_CE()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 1)));
}

// SET 1,A
void PreOpcode::pre_opcode_CF()
{
    this->cpu->regs.byte.A |= (0x01 << 1);
}

// SET 2,B
void PreOpcode::pre_opcode_D0()
{
    this->cpu->regs.byte.B |= (0x01 << 2);
}

// SET 2,C
void PreOpcode::pre_opcode_D1()
{
    this->cpu->regs.byte.C |= (0x01 << 2);
}

// SET 2,D
void PreOpcode::pre_opcode_D2()
{
    this->cpu->regs.byte.D |= (0x01 << 2);
}

// SET 2,E
void PreOpcode::pre_opcode_D3()
{
    this->cpu->regs.byte.E |= (0x01 << 2);
}

// SET 2,H
void PreOpcode::pre_opcode_D4()
{
    this->cpu->regs.byte.H |= (0x01 << 2);
}

// SET 2,L
void PreOpcode::pre_opcode_D5()
{
    this->cpu->regs.byte.L |= (0x01 << 2);
}

// SET 2,(HL)
void PreOpcode::pre_opcode_D6()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 2)));
}

// SET 2,A
void PreOpcode::pre_opcode_D7()
{
    this->cpu->regs.byte.A |= (0x01 << 2);
}

// SET 3,B
void PreOpcode::pre_opcode_D8()
{
    this->cpu->regs.byte.B |= (0x01 << 3);
}

// SET 3,C
void PreOpcode::pre_opcode_D9()
{
    this->cpu->regs.byte.C |= (0x01 << 3);
}

// SET 3,D
void PreOpcode::pre_opcode_DA()
{
    this->cpu->regs.byte.D |= (0x01 << 3);
}

// SET 3,E
void PreOpcode::pre_opcode_DB()
{
    this->cpu->regs.byte.E |= (0x01 << 3);
}

// SET 3,H
void PreOpcode::pre_opcode_DC()
{
    this->cpu->regs.byte.H |= (0x01 << 3);
}

// SET 3,L
void PreOpcode::pre_opcode_DD()
{
    this->cpu->regs.byte.L |= (0x01 << 3);
}

// SET 3,(HL)
void PreOpcode::pre_opcode_DE()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 3)));
}

// SET 3,A
void PreOpcode::pre_opcode_DF()
{
    this->cpu->regs.byte.A |= (0x01 << 3);
}

// SET 4,B
void PreOpcode::pre_opcode_E0()
{
    this->cpu->regs.byte.B |= (0x01 << 4);
}

// SET 4,C
void PreOpcode::pre_opcode_E1()
{
    this->cpu->regs.byte.C |= (0x01 << 4);
}

// SET 4,D
void PreOpcode::pre_opcode_E2()
{
    this->cpu->regs.byte.D |= (0x01 << 4);
}

// SET 4,E
void PreOpcode::pre_opcode_E3()
{
    this->cpu->regs.byte.E |= (0x01 << 4);
}

// SET 4,H
void PreOpcode::pre_opcode_E4()
{
    this->cpu->regs.byte.H |= (0x01 << 4);
}

// SET 4,L
void PreOpcode::pre_opcode_E5()
{
    this->cpu->regs.byte.L |= (0x01 << 4);
}

// SET 4,(HL)
void PreOpcode::pre_opcode_E6()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 4)));
}

// SET 4,A
void PreOpcode::pre_opcode_E7()
{
    this->cpu->regs.byte.A |= (0x01 << 4);
}

// SET 5,B
void PreOpcode::pre_opcode_E8()
{
    this->cpu->regs.byte.B |= (0x01 << 5);
}

// SET 5,C
void PreOpcode::pre_opcode_E9()
{
    this->cpu->regs.byte.C |= (0x01 << 5);
}

// SET 5,D
void PreOpcode::pre_opcode_EA()
{
    this->cpu->regs.byte.D |= (0x01 << 5);
}

// SET 5,E
void PreOpcode::pre_opcode_EB()
{
    this->cpu->regs.byte.E |= (0x01 << 5);
}

// SET 5,H
void PreOpcode::pre_opcode_EC()
{
    this->cpu->regs.byte.H |= (0x01 << 5);
}

// SET 5,L
void PreOpcode::pre_opcode_ED()
{
    this->cpu->regs.byte.L |= (0x01 << 5);
}

// SET 5,(HL)
void PreOpcode::pre_opcode_EE()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 5)));
}

// SET 5,A
void PreOpcode::pre_opcode_EF()
{
    this->cpu->regs.byte.A |= (0x01 << 5);
}

// SET 6,B
void PreOpcode::pre_opcode_F0()
{
    this->cpu->regs.byte.B |= (0x01 << 6);
}

// SET 6,C
void PreOpcode::pre_opcode_F1()
{
    this->cpu->regs.byte.C |= (0x01 << 6);
}

// SET 6,D
void PreOpcode::pre_opcode_F2()
{
    this->cpu->regs.byte.D |= (0x01 << 6);
}

// SET 6,E
void PreOpcode::pre_opcode_F3()
{
    this->cpu->regs.byte.E |= (0x01 << 6);
}

// SET 6,H
void PreOpcode::pre_opcode_F4()
{
    this->cpu->regs.byte.H |= (0x01 << 6);
}

// SET 6,L
void PreOpcode::pre_opcode_F5()
{
    this->cpu->regs.byte.L |= (0x01 << 6);
}

// SET 6,(HL)
void PreOpcode::pre_opcode_F6()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 6)));
}

// SET 6,A
void PreOpcode::pre_opcode_F7()
{
    this->cpu->regs.byte.A |= (0x01 << 6);
}

// SET 7,B
void PreOpcode::pre_opcode_F8()
{
    this->cpu->regs.byte.B |= (0x01 << 7);
}

// SET 7,C
void PreOpcode::pre_opcode_F9()
{
    this->cpu->regs.byte.C |= (0x01 << 7);
}

// SET 7,D
void PreOpcode::pre_opcode_FA()
{
    this->cpu->regs.byte.D |= (0x01 << 7);
}

// SET 7,E
void PreOpcode::pre_opcode_FB()
{
    this->cpu->regs.byte.E |= (0x01 << 7);
}

// SET 7,H
void PreOpcode::pre_opcode_FC()
{
    this->cpu->regs.byte.H |= (0x01 << 7);
}

// SET 7,L
void PreOpcode::pre_opcode_FD()
{
    this->cpu->regs.byte.L |= (0x01 << 7);
}

// SET 7,(HL)
void PreOpcode::pre_opcode_FE()
{
    this->memory->write_memory(this->cpu->regs.word.HL, (this->memory->read_memory(this->cpu->regs.word.HL) | (0x01 << 7)));
}

// SET 7,A
void PreOpcode::pre_opcode_FF()
{
    this->cpu->regs.byte.A |= (0x01 << 7);
}
