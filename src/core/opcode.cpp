#include "opcode.h"
#include "pre_opcode.h"
#include "system_types.h"
#include "bit_helper.h"

Opcode::Opcode(System * const system, Memory * const memory, Cpu * const cpu)
{
    this->memory = memory;
    this->cpu = cpu;
    this->system = system;
    this->preOpcode = new PreOpcode(memory, cpu);
}


Opcode::~Opcode()
{
    delete this->preOpcode;
}

void Opcode::next()
{
    this->opcode = this->memory->read_memory(this->cpu->pc.PC++);
}

int Opcode::current()
{
    return this->opcode;
}

void Opcode::INC_8(u8 * byte)
{
    this->cpu->reset_n();
    (*byte & 0xf) == 0xf ? this->cpu->set_h() : this->cpu->reset_h();
    ++(*byte);
    (*byte == 0) ? this->cpu->set_z() : this->cpu->reset_z();
}

void Opcode::DEC_8(u8 * byte)
{
    this->cpu->set_n();
    (*byte & 0xf) == 0 ? this->cpu->set_h() : this->cpu->reset_h();
    --(*byte);
    (*byte == 0) ? this->cpu->set_z() : this->cpu->reset_z();
}

void Opcode::ADD_HL(u16 word)
{
    this->cpu->reset_n();
    (((this->cpu->regs.word.HL & 0x0FFF) + (word & 0x0FFF)) & 0x1000) == 0x1000 ? this->cpu->set_h() : this->cpu->reset_h();
    int word1 = this->cpu->regs.word.HL;
    int word2 = word;
    (((word1 & 0xffff) + (word2 & 0xffff)) & 0x10000) == 0x10000 ? this->cpu->set_c() : this->cpu->reset_c();
    this->cpu->regs.word.HL += word;
}

void Opcode::ADD_A(u8 byte)
{
    BitHelper::half_carry(this->cpu->regs.byte.A, byte) ? this->cpu->set_h() : this->cpu->reset_h();
    u16 temp_byte1 = this->cpu->regs.byte.A;
    u16 temp_byte2 = byte;
    ((temp_byte1 + temp_byte2) & 0x0100) == 0x0100 ? this->cpu->set_c() : this->cpu->reset_c();
    this->cpu->regs.byte.A += byte;
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->reset_n();
}

void Opcode::ADC_A(u8 byte)
{
    int c = this->cpu->test_c();
    ((this->cpu->regs.byte.A & 0x0f) + (byte & 0xf) + c) > 0xf ? this->cpu->set_h() : this->cpu->reset_h();
    int result = this->cpu->regs.byte.A + byte + c;
    result > 0xff ? this->cpu->set_c() : this->cpu->reset_c();
    this->cpu->regs.byte.A += byte + c;
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->reset_n();
}

void Opcode::SUB_A(u8 byte)
{
    (this->cpu->regs.byte.A & 0x0F) < (byte & 0x0F) ? this->cpu->set_h() : this->cpu->reset_h();
    this->cpu->regs.byte.A < byte ? this->cpu->set_c() : this->cpu->reset_c();
    this->cpu->regs.byte.A -= byte;
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->set_n();
}

void Opcode::SBC_A(u8 byte)
{
    u8 c = this->cpu->test_c();
    (u16)this->cpu->regs.byte.A < ((u16)byte + c) ? this->cpu->set_c() : this->cpu->reset_c();
    (this->cpu->regs.byte.A & 0x0f) < ((byte &0x0f) + c) ? this->cpu->set_h() : this->cpu->reset_h();
    this->cpu->regs.byte.A -= (byte + c);
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->set_n();
}

void Opcode::AND_A(u8 byte)
{
    this->cpu->regs.byte.A &= byte;
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->set_h();
    this->cpu->reset_c();
}

void Opcode::XOR_A(u8 byte)
{
    this->cpu->regs.byte.A ^= byte;
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->reset_h();
    this->cpu->reset_c();
}

void Opcode::OR_A(u8 byte)
{
    this->cpu->regs.byte.A |= byte;
    this->cpu->regs.byte.A == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->reset_h();
    this->cpu->reset_c();
}

void Opcode::CP_A(u8 byte)
{
    (this->cpu->regs.byte.A - byte) == 0 ? this->cpu->set_z() : this->cpu->reset_z();
    this->cpu->regs.byte.A < byte ? this->cpu->set_c() : this->cpu->reset_c();
    (this->cpu->regs.byte.A & 0x0F) < (byte & 0x0F) ? this->cpu->set_h() : this->cpu->reset_h();
    this->cpu->set_n();
}

// LD BC, d16
void Opcode::opcode_01()
{
    this->cpu->regs.byte.C = this->memory->read_memory(this->cpu->pc.PC++);
    this->cpu->regs.byte.B = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD DE, d16
void Opcode::opcode_11()
{
    this->cpu->regs.byte.E = this->memory->read_memory(this->cpu->pc.PC++);
    this->cpu->regs.byte.D = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD HL, d16
void Opcode::opcode_21()
{
    this->cpu->regs.byte.L = this->memory->read_memory(this->cpu->pc.PC++);
    this->cpu->regs.byte.H = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD SP, d16
void Opcode::opcode_31()
{
    this->cpu->sp.byte.L = this->memory->read_memory(this->cpu->pc.PC++);
    this->cpu->sp.byte.H = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD (BC), A
void Opcode::opcode_02()
{
    this->memory->write_memory(this->cpu->regs.word.BC, this->cpu->regs.byte.A);
}

// LD (DE), A
void Opcode::opcode_12()
{
    this->memory->write_memory(this->cpu->regs.word.DE, this->cpu->regs.byte.A);
}

// LD (HL+), A
void Opcode::opcode_22()
{
    this->memory->write_memory(this->cpu->regs.word.HL++, this->cpu->regs.byte.A);
}

// LD (HL-), A
void Opcode::opcode_32()
{
    this->memory->write_memory(this->cpu->regs.word.HL--, this->cpu->regs.byte.A);
}

// LD B, d8
void Opcode::opcode_06()
{
    this->cpu->regs.byte.B = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD D, d8
void Opcode::opcode_16()
{
    this->cpu->regs.byte.D = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD H, d8
void Opcode::opcode_26()
{
    this->cpu->regs.byte.H = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD (HL), d8
void Opcode::opcode_36()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->memory->read_memory(this->cpu->pc.PC++));
}

// LD (a16), SP
void Opcode::opcode_08()
{
    u16 lower = this->memory->read_memory(this->cpu->pc.PC++);
    u16 higher = this->memory->read_memory(this->cpu->pc.PC++);
    u16 address = (higher << 8) | lower;
    this->memory->write_memory(address++, this->cpu->sp.byte.L);
    this->memory->write_memory(address, this->cpu->sp.byte.H);
}

// LD A, (BC)
void Opcode::opcode_0A()
{
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->regs.word.BC);
}

// LD A, (DE)
void Opcode::opcode_1A()
{
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->regs.word.DE);
}

// LD A, (HL+)
void Opcode::opcode_2A()
{
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->regs.word.HL++);
}

// LD A, (HL-)
void Opcode::opcode_3A()
{
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->regs.word.HL--);
}

// LD C, d8
void Opcode::opcode_0E()
{
    this->cpu->regs.byte.C = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD E, d8
void Opcode::opcode_1E()
{
    this->cpu->regs.byte.E = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD L, d8
void Opcode::opcode_2E()
{
    this->cpu->regs.byte.L = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD A, d8
void Opcode::opcode_3E()
{
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->pc.PC++);
}

// LD B, B
void Opcode::opcode_40()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.B;
}

// LD B, C
void Opcode::opcode_41()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.C;
}

// LD B, D
void Opcode::opcode_42()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.D;
}

// LD B, E
void Opcode::opcode_43()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.E;
}

// LD B, H
void Opcode::opcode_44()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.H;
}

// LD B, L
void Opcode::opcode_45()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.L;
}

// LD B, (HL)
void Opcode::opcode_46()
{
    this->cpu->regs.byte.B = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD B, A
void Opcode::opcode_47()
{
    this->cpu->regs.byte.B = this->cpu->regs.byte.A;
}

// LD C, B
void Opcode::opcode_48()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.B;
}

// LD C, C
void Opcode::opcode_49()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.C;
}

// LD C, D
void Opcode::opcode_4A()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.D;
}

// LD C, E
void Opcode::opcode_4B()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.E;
}

// LD C, H
void Opcode::opcode_4C()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.H;
}

// LD C, L
void Opcode::opcode_4D()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.L;
}

// LD C, (HL)
void Opcode::opcode_4E()
{
    this->cpu->regs.byte.C = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD C, A
void Opcode::opcode_4F()
{
    this->cpu->regs.byte.C = this->cpu->regs.byte.A;
}

// LD D, B
void Opcode::opcode_50()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.B;
}

// LD D, C
void Opcode::opcode_51()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.C;
}

// LD D, D
void Opcode::opcode_52()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.D;
}

// LD D, E
void Opcode::opcode_53()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.E;
}

// LD D, H
void Opcode::opcode_54()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.H;
}

// LD D, L
void Opcode::opcode_55()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.L;
}

// LD D, (HL)
void Opcode::opcode_56()
{
    this->cpu->regs.byte.D = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD D, A
void Opcode::opcode_57()
{
    this->cpu->regs.byte.D = this->cpu->regs.byte.A;
}

// LD E, B
void Opcode::opcode_58()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.B;
}

// LD E, C
void Opcode::opcode_59()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.C;
}

// LD E, D
void Opcode::opcode_5A()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.D;
}

// LD E, E
void Opcode::opcode_5B()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.E;
}

// LD E, H
void Opcode::opcode_5C()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.H;
}

// LD E, L
void Opcode::opcode_5D()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.L;
}

// LD E, (HL)
void Opcode::opcode_5E()
{
    this->cpu->regs.byte.E = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD E, A
void Opcode::opcode_5F()
{
    this->cpu->regs.byte.E = this->cpu->regs.byte.A;
}

// LD H, B
void Opcode::opcode_60()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.B;
}

// LD H, C
void Opcode::opcode_61()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.C;
}

// LD H, D
void Opcode::opcode_62()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.D;
}

// LD H, E
void Opcode::opcode_63()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.E;
}

// LD H, H
void Opcode::opcode_64()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.H;
}

// LD H, L
void Opcode::opcode_65()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.L;
}

// LD H, (HL)
void Opcode::opcode_66()
{
    this->cpu->regs.byte.H = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD H, A
void Opcode::opcode_67()
{
    this->cpu->regs.byte.H = this->cpu->regs.byte.A;
}

// LD L, B
void Opcode::opcode_68()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.B;
}

// LD L, C
void Opcode::opcode_69()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.C;
}

// LD L, D
void Opcode::opcode_6A()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.D;
}

// LD L, E
void Opcode::opcode_6B()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.E;
}

// LD L, H
void Opcode::opcode_6C()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.H;
}

// LD L, L
void Opcode::opcode_6D()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.L;
}

// LD L, (HL)
void Opcode::opcode_6E()
{
    this->cpu->regs.byte.L = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD L, A
void Opcode::opcode_6F()
{
    this->cpu->regs.byte.L = this->cpu->regs.byte.A;
}

// LD (HL), B
void Opcode::opcode_70()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.B);
}

// LD (HL), C
void Opcode::opcode_71()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.C);
}

// LD (HL), D
void Opcode::opcode_72()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.D);
}

// LD (HL), E
void Opcode::opcode_73()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.E);
}

// LD (HL), H
void Opcode::opcode_74()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.H);
}

// LD (HL), L
void Opcode::opcode_75()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.L);
}

// LD (HL), A
void Opcode::opcode_77()
{
    this->memory->write_memory(this->cpu->regs.word.HL, this->cpu->regs.byte.A);
}

// LD A, B
void Opcode::opcode_78()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.B;
}

// LD A, C
void Opcode::opcode_79()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.C;
}

// LD A, D
void Opcode::opcode_7A()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.D;
}

// LD A, E
void Opcode::opcode_7B()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.E;
}

// LD A, H
void Opcode::opcode_7C()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.H;
}

// LD A, L
void Opcode::opcode_7D()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.L;
}

// LD A, (HL)
void Opcode::opcode_7E()
{
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->regs.word.HL);
}

// LD A, A
void Opcode::opcode_7F()
{
    this->cpu->regs.byte.A = this->cpu->regs.byte.A;
}

// LDH (a8), A
void Opcode::opcode_E0()
{
    u8 temp = this->memory->read_memory(this->cpu->pc.PC++);
    this->memory->write_memory(0xFF00+temp, this->cpu->regs.byte.A);
}

// LDH A, (a8)
void Opcode::opcode_F0()
{
    u8 temp = this->memory->read_memory(this->cpu->pc.PC++);
    this->cpu->regs.byte.A = this->memory->read_memory(0xFF00+temp);
}

// LD (C), A
void Opcode::opcode_E2()
{
    this->memory->write_memory(0xFF00+this->cpu->regs.byte.C, this->cpu->regs.byte.A);
}

// LD A, (C)
void Opcode::opcode_F2()
{
    this->cpu->regs.byte.A = this->memory->read_memory(0xFF00+this->cpu->regs.byte.C);
}

// LD SP, HL
void Opcode::opcode_F9()
{
    this->cpu->sp.SP = this->cpu->regs.word.HL;
}

// LD (a16), A
void Opcode::opcode_EA()
{
    u16 lsb = this->memory->read_memory(this->cpu->pc.PC++);
    u16 msb = this->memory->read_memory(this->cpu->pc.PC++);
    msb <<= 8;
    u16 address = msb | lsb;
    this->memory->write_memory(address, this->cpu->regs.byte.A);
}

// LD A, (a16)
void Opcode::opcode_FA()
{
    u16 lsb = this->memory->read_memory(this->cpu->pc.PC++);
    u16 msb = this->memory->read_memory(this->cpu->pc.PC++);
    msb <<= 8;
    u16 address = msb | lsb;
    this->cpu->regs.byte.A = this->memory->read_memory(address);
}

// JR NZ, r8
void Opcode::opcode_20()
{
    if (this->cpu->test_z() == 0) {
        u8 byte = this->memory->read_memory(this->cpu->pc.PC++);
        this->cpu->pc.PC += (signed char)byte;
    } else {
        this->cpu->pc.PC++;
        this->system->counter -= 4;
    }
}

// JR NC, r8
void Opcode::opcode_30()
{
    if (this->cpu->test_c() == 0) {
        u8 byte = this->memory->read_memory(this->cpu->pc.PC++);
        this->cpu->pc.PC += (signed char)byte;
    } else {
        this->cpu->pc.PC++;
        this->system->counter -= 4;
    }
}

// JR r8
void Opcode::opcode_18()
{
    u8 byte = this->memory->read_memory(this->cpu->pc.PC++);
    this->cpu->pc.PC += (signed char)byte;
}

// JR Z, r8
void Opcode::opcode_28()
{
    if (this->cpu->test_z()) {
        u8 byte = this->memory->read_memory(this->cpu->pc.PC++);
        this->cpu->pc.PC += (signed char)byte;
    } else {
        this->cpu->pc.PC++;
        this->system->counter -= 4;
    }
}

// JR C, r8
void Opcode::opcode_38()
{
    if (this->cpu->test_c()) {
        u8 byte = this->memory->read_memory(this->cpu->pc.PC++);
        this->cpu->pc.PC += (signed char)byte;
    } else {
        this->cpu->pc.PC++;
        this->system->counter -= 4;
    }
}

// RET NZ
void Opcode::opcode_C0()
{
    if (this->cpu->test_z() == 0) {
        this->cpu->pc.byte.L = this->memory->read_memory(this->cpu->sp.SP);
        this->cpu->pc.byte.H = this->memory->read_memory(this->cpu->sp.SP+1);
        this->cpu->sp.SP += 2;
    } else {
        this->system->counter -= 12;
    }
}

// RET NC
void Opcode::opcode_D0()
{
    if (this->cpu->test_c() == 0) {
        this->cpu->pc.byte.L = this->memory->read_memory(this->cpu->sp.SP);
        this->cpu->pc.byte.H = this->memory->read_memory(this->cpu->sp.SP+1);
        this->cpu->sp.SP += 2;
    } else {
        this->system->counter -= 12;
    }
}

// JP NZ, a16
void Opcode::opcode_C2()
{
    if (this->cpu->test_z() == 0) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC+1);
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 4;
    }
}

// JP NC, a16
void Opcode::opcode_D2()
{
    if (this->cpu->test_c() == 0) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC+1);
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 4;
    }
}

// JP a16
void Opcode::opcode_C3()
{
    u8 lsb = this->memory->read_memory(this->cpu->pc.PC);
    u8 msb = this->memory->read_memory(this->cpu->pc.PC+1);
    this->cpu->pc.byte.L = lsb;
    this->cpu->pc.byte.H = msb;
}

// CALL NZ, a16
void Opcode::opcode_C4()
{
    if (this->cpu->test_z() == 0) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC++);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC++);
        this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
        this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
        this->cpu->sp.SP -= 2;
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 12;
    }
}

// CALL NC, a16
void Opcode::opcode_D4()
{
    if (this->cpu->test_c() == 0) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC++);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC++);
        this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
        this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
        this->cpu->sp.SP -= 2;
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 12;
    }
}

// RST 00H
void Opcode::opcode_C7()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x00;
}

// RST 10H
void Opcode::opcode_D7()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x10;
}

// RST 20H
void Opcode::opcode_E7()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x20;
}

// RST 30H
void Opcode::opcode_F7()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x30;
}

// RET Z
void Opcode::opcode_C8()
{
    if (this->cpu->test_z()) {
        this->cpu->pc.byte.L = this->memory->read_memory(this->cpu->sp.SP++);
        this->cpu->pc.byte.H = this->memory->read_memory(this->cpu->sp.SP++);
    } else {
        this->system->counter -= 12;
    }
}

// RET C
void Opcode::opcode_D8()
{
    if (this->cpu->test_c()) {
        this->cpu->pc.byte.L = this->memory->read_memory(this->cpu->sp.SP++);
        this->cpu->pc.byte.H = this->memory->read_memory(this->cpu->sp.SP++);
    } else {
        this->system->counter -= 12;
    }
}

// RET
void Opcode::opcode_C9()
{
    this->cpu->pc.byte.L = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->pc.byte.H = this->memory->read_memory(this->cpu->sp.SP++);
}

// RETI
void Opcode::opcode_D9()
{
    this->cpu->pc.byte.L = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->pc.byte.H = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->ime = 1;
}

// JP (HL)
void Opcode::opcode_E9()
{
    this->cpu->pc.PC = this->cpu->regs.word.HL;
}

// JP Z, a16
void Opcode::opcode_CA()
{
    if (this->cpu->test_z()) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC+1);
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 4;
    }
}

// JP C, a16
void Opcode::opcode_DA()
{
    if (this->cpu->test_c()) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC+1);
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 4;
    }
}

// CALL Z, a16
void Opcode::opcode_CC()
{
    if (this->cpu->test_z()) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC++);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC++);
        this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
        this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
        this->cpu->sp.SP -= 2;
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 12;
    }
}

// CALL C, a16
void Opcode::opcode_DC()
{
    if (this->cpu->test_c()) {
        u8 lsb = this->memory->read_memory(this->cpu->pc.PC++);
        u8 msb = this->memory->read_memory(this->cpu->pc.PC++);
        this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
        this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
        this->cpu->sp.SP -= 2;
        this->cpu->pc.byte.L = lsb;
        this->cpu->pc.byte.H = msb;
    } else {
        this->cpu->pc.PC += 2;
        this->system->counter -= 12;
    }
}

// CALL a16
void Opcode::opcode_CD()
{
    u8 lsb = this->memory->read_memory(this->cpu->pc.PC++);
    u8 msb = this->memory->read_memory(this->cpu->pc.PC++);
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.byte.L = lsb;
    this->cpu->pc.byte.H = msb;
}

// RST 08H
void Opcode::opcode_CF()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x08;
}

// RST 18H
void Opcode::opcode_DF()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x18;
}

// RST 28H
void Opcode::opcode_EF()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x28;
}

// RST 38H
void Opcode::opcode_FF()
{
    this->memory->write_memory(this->cpu->sp.SP-1, this->cpu->pc.byte.H);
    this->memory->write_memory(this->cpu->sp.SP-2, this->cpu->pc.byte.L);
    this->cpu->sp.SP -= 2;
    this->cpu->pc.PC = 0x0000 + 0x38;
}

// INC BC
void Opcode::opcode_03()
{
    ++this->cpu->regs.word.BC;
}

// INC DE
void Opcode::opcode_13()
{
    ++this->cpu->regs.word.DE;
}

// INC HL
void Opcode::opcode_23()
{
    ++this->cpu->regs.word.HL;
}

// INC SP
void Opcode::opcode_33()
{
    ++this->cpu->sp.SP;
}

// INC B
void Opcode::opcode_04()
{
    INC_8(&this->cpu->regs.byte.B);
}

// INC D
void Opcode::opcode_14()
{
    INC_8(&this->cpu->regs.byte.D);
}

// INC H
void Opcode::opcode_24()
{
    INC_8(&this->cpu->regs.byte.H);
}

// INC (HL)
void Opcode::opcode_34()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    INC_8(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// INC C
void Opcode::opcode_0C()
{
    INC_8(&this->cpu->regs.byte.C);
}

// INC E
void Opcode::opcode_1C()
{
    INC_8(&this->cpu->regs.byte.E);
}

// INC L
void Opcode::opcode_2C()
{
    INC_8(&this->cpu->regs.byte.L);
}

// INC A
void Opcode::opcode_3C()
{
    INC_8(&this->cpu->regs.byte.A);
}

// DEC B
void Opcode::opcode_05()
{
    DEC_8(&this->cpu->regs.byte.B);
}

// DEC D
void Opcode::opcode_15()
{
    DEC_8(&this->cpu->regs.byte.D);
}

// DEC H
void Opcode::opcode_25()
{
    DEC_8(&this->cpu->regs.byte.H);
}

// DEC (HL)
void Opcode::opcode_35()
{
    u8 byte = this->memory->read_memory(this->cpu->regs.word.HL);
    DEC_8(&byte);
    this->memory->write_memory(this->cpu->regs.word.HL, byte);
}

// DEC BC
void Opcode::opcode_0B()
{
    --this->cpu->regs.word.BC;
}

// DEC DE
void Opcode::opcode_1B()
{
    --this->cpu->regs.word.DE;
}

// DEC HL
void Opcode::opcode_2B()
{
    --this->cpu->regs.word.HL;
}

// DEC SP
void Opcode::opcode_3B()
{
    --this->cpu->sp.SP;
}

// DEC C
void Opcode::opcode_0D()
{
    DEC_8(&this->cpu->regs.byte.C);
}

// DEC E
void Opcode::opcode_1D()
{
    DEC_8(&this->cpu->regs.byte.E);
}

// DEC L
void Opcode::opcode_2D()
{
    DEC_8(&this->cpu->regs.byte.L);
}

// DEC A
void Opcode::opcode_3D()
{
    DEC_8(&this->cpu->regs.byte.A);
}

// RLCA
void Opcode::opcode_07()
{
    u8 bit = (this->cpu->regs.byte.A >> 7) & 1; // get 7th bit
    this->cpu->regs.byte.A = this->cpu->regs.byte.A << 1; // shift left 1
    this->cpu->regs.byte.A |= bit;
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    this->cpu->reset_z();
    this->cpu->reset_h();
    this->cpu->reset_n();
}

// RLA
void Opcode::opcode_17()
{
    u8 bit = (this->cpu->regs.byte.A >> 7) & 1; // get 7th bit
    u8 carry = (this->cpu->regs.byte.F >> 4) & 1; // get carry bit
    this->cpu->regs.byte.A = this->cpu->regs.byte.A << 1; // shift left 1
    this->cpu->regs.byte.A |= carry;
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->reset_h();
}

// DAA
void Opcode::opcode_27()
{
    int a = this->cpu->regs.byte.A;
    if (this->cpu->test_n() == 0) {
        if ((a & 0x0f) > 0x09 || this->cpu->test_h() == 1) {
            a += 0x06;
        }
        if (a > 0x9f || this->cpu->test_c() == 1) {
            a += 0x60;
        }
    } else {
        if (this->cpu->test_h()) {
            a = (a - 0x06) & 0xff;
        }
        if (this->cpu->test_c()) {
            a -= 0x60;
        }
    }
    this->cpu->reset_h();
    if ((a & 0x100) == 0x100) {
        this->cpu->set_c();
    }
    this->cpu->regs.byte.A = (a & 0xff);
    if (this->cpu->regs.byte.A == 0) {
        this->cpu->set_z();
    } else {
        this->cpu->reset_z();
    }
}

// SCF
void Opcode::opcode_37()
{
    this->cpu->set_c();
    this->cpu->reset_h();
    this->cpu->reset_n();
}

// RRCA
void Opcode::opcode_0F()
{
    u8 bit = (this->cpu->regs.byte.A) & 1; // get 0th bit
    this->cpu->regs.byte.A = this->cpu->regs.byte.A >> 1; // shift left 1
    this->cpu->regs.byte.A |= (bit << 7);
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    this->cpu->reset_z();
    this->cpu->reset_h();
    this->cpu->reset_n();
}

// RRA
void Opcode::opcode_1F()
{
    u8 bit = (this->cpu->regs.byte.A) & 1; // get 0th bit
    u8 carry = (this->cpu->regs.byte.F >> 4) & 1; // get carry bit
    this->cpu->regs.byte.A = this->cpu->regs.byte.A >> 1; // shift left 1
    this->cpu->regs.byte.A |= (carry << 7);
    if (bit == 1) {
        this->cpu->set_c();
    } else {
        this->cpu->reset_c();
    }
    this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->reset_h();
}

// CPL
void Opcode::opcode_2F()
{
    this->cpu->regs.byte.A = ~this->cpu->regs.byte.A;
    this->cpu->set_n();
    this->cpu->set_h();
}

// CCF
void Opcode::opcode_3F()
{
    if (this->cpu->test_c()) {
        this->cpu->reset_c();
    } else {
        this->cpu->set_c();
    }
    this->cpu->reset_h();
    this->cpu->reset_n();
}

// ADD HL, BC
void Opcode::opcode_09()
{
    ADD_HL(this->cpu->regs.word.BC);
}

// ADD HL, DE
void Opcode::opcode_19()
{
    ADD_HL(this->cpu->regs.word.DE);
}

// ADD HL, HL
void Opcode::opcode_29()
{
    ADD_HL(this->cpu->regs.word.HL);
}

// ADD HL, SP
void Opcode::opcode_39()
{
    ADD_HL(this->cpu->sp.SP);
}

// ADD SP, r8
void Opcode::opcode_E8()
{
    this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->reset_h();
    this->cpu->reset_c();
    u8 n = this->memory->read_memory(this->cpu->pc.PC++);
    if ((((this->cpu->sp.SP & 0xf) + (n & 0xf)) & 0x10) == 0x10) {
        this->cpu->set_h();
    }
    if ((((this->cpu->sp.SP & 0xff) + (n & 0xff)) & 0x0100) == 0x0100) {
        this->cpu->set_c();
    }
    this->cpu->sp.SP += (signed char)n;
}

// LD HL, SP+r8
void Opcode::opcode_F8()
{
    this->cpu->reset_z();
    this->cpu->reset_n();
    this->cpu->reset_h();
    this->cpu->reset_c();
    u8 n = this->memory->read_memory(this->cpu->pc.PC++);
    if ((((this->cpu->sp.SP & 0xf) + (n & 0xf)) & 0x10) == 0x10) {
        this->cpu->set_h();
    }
    if ((((this->cpu->sp.SP & 0xff) + (n & 0xff)) & 0x0100) == 0x0100) {
        this->cpu->set_c();
    }
    u16 address = this->cpu->sp.SP + (signed char)n;
    this->cpu->regs.word.HL = address;
}

// ADD A, B
void Opcode::opcode_80()
{
    ADD_A(this->cpu->regs.byte.B);
}

// ADD A, C
void Opcode::opcode_81()
{
    ADD_A(this->cpu->regs.byte.C);
}

// ADD A, D
void Opcode::opcode_82()
{
    ADD_A(this->cpu->regs.byte.D);
}

// ADD A, E
void Opcode::opcode_83()
{
    ADD_A(this->cpu->regs.byte.E);
}

// ADD A, H
void Opcode::opcode_84()
{
    ADD_A(this->cpu->regs.byte.H);
}

// ADD A, L
void Opcode::opcode_85()
{
    ADD_A(this->cpu->regs.byte.L);
}

// ADD A, (HL)
void Opcode::opcode_86()
{
    ADD_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// ADD A, A
void Opcode::opcode_87()
{
    ADD_A(this->cpu->regs.byte.A);
}

// ADD A, d8
void Opcode::opcode_C6()
{
    ADD_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// ADC A, B
void Opcode::opcode_88()
{
    ADC_A(this->cpu->regs.byte.B);
}

// ADC A, C
void Opcode::opcode_89()
{
    ADC_A(this->cpu->regs.byte.C);
}

// ADC A, D
void Opcode::opcode_8A()
{
    ADC_A(this->cpu->regs.byte.D);
}

// ADC A, E
void Opcode::opcode_8B()
{
    ADC_A(this->cpu->regs.byte.E);
}

// ADC A, H
void Opcode::opcode_8C()
{
    ADC_A(this->cpu->regs.byte.H);
}

// ADC A, L
void Opcode::opcode_8D()
{
    ADC_A(this->cpu->regs.byte.L);
}

// ADC A, (HL)
void Opcode::opcode_8E()
{
   ADC_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// ADC A, A
void Opcode::opcode_8F()
{
    ADC_A(this->cpu->regs.byte.A);
}

// ADC A, d8
void Opcode::opcode_CE()
{
    ADC_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// SUB B
void Opcode::opcode_90()
{
    SUB_A(this->cpu->regs.byte.B);
}

// SUB C
void Opcode::opcode_91()
{
    SUB_A(this->cpu->regs.byte.C);
}

// SUB D
void Opcode::opcode_92()
{
    SUB_A(this->cpu->regs.byte.D);
}

// SUB E
void Opcode::opcode_93()
{
    SUB_A(this->cpu->regs.byte.E);
}

// SUB H
void Opcode::opcode_94()
{
    SUB_A(this->cpu->regs.byte.H);
}

// SUB L
void Opcode::opcode_95()
{
    SUB_A(this->cpu->regs.byte.L);
}

// SUB (HL)
void Opcode::opcode_96()
{
    SUB_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// SUB A
void Opcode::opcode_97()
{
    this->cpu->regs.byte.A = 0;
    this->cpu->reset_h();
    this->cpu->reset_c();
    this->cpu->set_z();
    this->cpu->set_n();
}

// SUB d8
void Opcode::opcode_D6()
{
    SUB_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// SBC A, B
void Opcode::opcode_98()
{
    SBC_A(this->cpu->regs.byte.B);
}

// SBC A, C
void Opcode::opcode_99()
{
    SBC_A(this->cpu->regs.byte.C);
}

// SBC A, D
void Opcode::opcode_9A()
{
    SBC_A(this->cpu->regs.byte.D);
}

// SBC A, E
void Opcode::opcode_9B()
{
    SBC_A(this->cpu->regs.byte.E);
}

// SBC A, H
void Opcode::opcode_9C()
{
    SBC_A(this->cpu->regs.byte.H);
}

// SBC A, L
void Opcode::opcode_9D()
{
    SBC_A(this->cpu->regs.byte.L);
}

// SBC A, (HL)
void Opcode::opcode_9E()
{
    SBC_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// SBC A, A
void Opcode::opcode_9F()
{
    SBC_A(this->cpu->regs.byte.A);
}

// SBC A, d8
void Opcode::opcode_DE()
{
    SBC_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// AND B
void Opcode::opcode_A0()
{
    AND_A(this->cpu->regs.byte.B);
}

// AND C
void Opcode::opcode_A1()
{
    AND_A(this->cpu->regs.byte.C);
}

// AND D
void Opcode::opcode_A2()
{
    AND_A(this->cpu->regs.byte.D);
}

// AND E
void Opcode::opcode_A3()
{
    AND_A(this->cpu->regs.byte.E);
}

// AND H
void Opcode::opcode_A4()
{
    AND_A(this->cpu->regs.byte.H);
}

// AND L
void Opcode::opcode_A5()
{
    AND_A(this->cpu->regs.byte.L);
}

// AND (HL)
void Opcode::opcode_A6()
{
    AND_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// AND A
void Opcode::opcode_A7()
{
    AND_A(this->cpu->regs.byte.A);
}

// AND d8
void Opcode::opcode_E6()
{
    AND_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// XOR B
void Opcode::opcode_A8()
{
    XOR_A(this->cpu->regs.byte.B);
}

// XOR C
void Opcode::opcode_A9()
{
    XOR_A(this->cpu->regs.byte.C);
}

// XOR D
void Opcode::opcode_AA()
{
    XOR_A(this->cpu->regs.byte.D);
}

// XOR E
void Opcode::opcode_AB()
{
    XOR_A(this->cpu->regs.byte.E);
}

// XOR H
void Opcode::opcode_AC()
{
    XOR_A(this->cpu->regs.byte.H);
}

// XOR L
void Opcode::opcode_AD()
{
    XOR_A(this->cpu->regs.byte.L);
}

// XOR (HL)
void Opcode::opcode_AE()
{
    XOR_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// XOR A
void Opcode::opcode_AF()
{
    XOR_A(this->cpu->regs.byte.A);
}

// XOR d8
void Opcode::opcode_EE()
{
    XOR_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// OR B
void Opcode::opcode_B0()
{
    OR_A(this->cpu->regs.byte.B);
}

// OR C
void Opcode::opcode_B1()
{
    OR_A(this->cpu->regs.byte.C);
}

// OR D
void Opcode::opcode_B2()
{
    OR_A(this->cpu->regs.byte.D);
}

// OR E
void Opcode::opcode_B3()
{
    OR_A(this->cpu->regs.byte.E);
}

// OR H
void Opcode::opcode_B4()
{
    OR_A(this->cpu->regs.byte.H);
}

// OR L
void Opcode::opcode_B5()
{
    OR_A(this->cpu->regs.byte.L);
}

// OR (HL)
void Opcode::opcode_B6()
{
    OR_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// OR A
void Opcode::opcode_B7()
{
    OR_A(this->cpu->regs.byte.A);
}

// OR d8
void Opcode::opcode_F6()
{
    OR_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// CP B
void Opcode::opcode_B8()
{
    CP_A(this->cpu->regs.byte.B);
}

// CP C
void Opcode::opcode_B9()
{
    CP_A(this->cpu->regs.byte.C);
}

// CP D
void Opcode::opcode_BA()
{
    CP_A(this->cpu->regs.byte.D);
}

// CP E
void Opcode::opcode_BB()
{
    CP_A(this->cpu->regs.byte.E);
}

// CP H
void Opcode::opcode_BC()
{
    CP_A(this->cpu->regs.byte.H);
}

// CP L
void Opcode::opcode_BD()
{
    CP_A(this->cpu->regs.byte.L);
}

// CP (HL)
void Opcode::opcode_BE()
{
    CP_A(this->memory->read_memory(this->cpu->regs.word.HL));
}

// CP A
void Opcode::opcode_BF()
{
    this->cpu->set_z();
    this->cpu->reset_h();
    this->cpu->reset_c();
    this->cpu->set_n();
}

// CP d8
void Opcode::opcode_FE()
{
    CP_A(this->memory->read_memory(this->cpu->pc.PC++));
}

// POP BC
void Opcode::opcode_C1()
{
    this->cpu->regs.byte.C = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->regs.byte.B = this->memory->read_memory(this->cpu->sp.SP++);
}

// POP DE
void Opcode::opcode_D1()
{
    this->cpu->regs.byte.E = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->regs.byte.D = this->memory->read_memory(this->cpu->sp.SP++);
}

// POP HL
void Opcode::opcode_E1()
{
    this->cpu->regs.byte.L = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->regs.byte.H = this->memory->read_memory(this->cpu->sp.SP++);
}

// POP AF
void Opcode::opcode_F1()
{
    this->cpu->regs.byte.F = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->regs.byte.A = this->memory->read_memory(this->cpu->sp.SP++);
    this->cpu->regs.byte.F &= 0xf0;
}

void Opcode::push(u16 word)
{
    u8 high = (word & 0xff00) >> 8;
    u8 low = (word & 0x00ff);
    this->memory->write_memory(this->cpu->sp.SP-1, high);
    this->memory->write_memory(this->cpu->sp.SP-2, low);
    this->cpu->sp.SP = this->cpu->sp.SP - 2;
}

// PUSH BC
void Opcode::opcode_C5()
{
    push(this->cpu->regs.word.BC);
}

// PUSH DE
void Opcode::opcode_D5()
{
    push(this->cpu->regs.word.DE);
}

// PUSH HL
void Opcode::opcode_E5()
{
    push(this->cpu->regs.word.HL);
}

// PUSH AF
void Opcode::opcode_F5()
{
    push(this->cpu->regs.word.AF);
}

// STOP 0
void Opcode::opcode_10()
{
    this->cpu->pc.PC++;
    if (this->cpu->prepare_speed) {
        this->cpu->double_speed = !this->cpu->double_speed;
        this->cpu->prepare_speed = 0;
        this->memory->write_memory(0xFF4D, this->cpu->double_speed ? 0xFE : 0x7E);
    } else {
        this->cpu->stopped = 1;
    }
    this->memory->write_memory(0xff04, 0); // reset timer
}

// HALT TODO
void Opcode::opcode_76()
{
    this->cpu->halt = 1;
}

// PREFIX
void Opcode::opcode_CB()
{
    this->opcode = this->memory->read_memory(this->cpu->pc.PC++);
    this->system->counter += this->system->pre_cycles[this->opcode];
    switch (this->opcode) {
        case 0x00: this->preOpcode->pre_opcode_00(); break;
        case 0x01: this->preOpcode->pre_opcode_01(); break;
        case 0x02: this->preOpcode->pre_opcode_02(); break;
        case 0x03: this->preOpcode->pre_opcode_03(); break;
        case 0x04: this->preOpcode->pre_opcode_04(); break;
        case 0x05: this->preOpcode->pre_opcode_05(); break;
        case 0x06: this->preOpcode->pre_opcode_06(); break;
        case 0x07: this->preOpcode->pre_opcode_07(); break;
        case 0x08: this->preOpcode->pre_opcode_08(); break;
        case 0x09: this->preOpcode->pre_opcode_09(); break;
        case 0x0A: this->preOpcode->pre_opcode_0A(); break;
        case 0x0B: this->preOpcode->pre_opcode_0B(); break;
        case 0x0C: this->preOpcode->pre_opcode_0C(); break;
        case 0x0D: this->preOpcode->pre_opcode_0D(); break;
        case 0x0E: this->preOpcode->pre_opcode_0E(); break;
        case 0x0F: this->preOpcode->pre_opcode_0F(); break;
        case 0x10: this->preOpcode->pre_opcode_10(); break;
        case 0x11: this->preOpcode->pre_opcode_11(); break;
        case 0x12: this->preOpcode->pre_opcode_12(); break;
        case 0x13: this->preOpcode->pre_opcode_13(); break;
        case 0x14: this->preOpcode->pre_opcode_14(); break;
        case 0x15: this->preOpcode->pre_opcode_15(); break;
        case 0x16: this->preOpcode->pre_opcode_16(); break;
        case 0x17: this->preOpcode->pre_opcode_17(); break;
        case 0x18: this->preOpcode->pre_opcode_18(); break;
        case 0x19: this->preOpcode->pre_opcode_19(); break;
        case 0x1A: this->preOpcode->pre_opcode_1A(); break;
        case 0x1B: this->preOpcode->pre_opcode_1B(); break;
        case 0x1C: this->preOpcode->pre_opcode_1C(); break;
        case 0x1D: this->preOpcode->pre_opcode_1D(); break;
        case 0x1E: this->preOpcode->pre_opcode_1E(); break;
        case 0x1F: this->preOpcode->pre_opcode_1F(); break;
        case 0x20: this->preOpcode->pre_opcode_20(); break;
        case 0x21: this->preOpcode->pre_opcode_21(); break;
        case 0x22: this->preOpcode->pre_opcode_22(); break;
        case 0x23: this->preOpcode->pre_opcode_23(); break;
        case 0x24: this->preOpcode->pre_opcode_24(); break;
        case 0x25: this->preOpcode->pre_opcode_25(); break;
        case 0x26: this->preOpcode->pre_opcode_26(); break;
        case 0x27: this->preOpcode->pre_opcode_27(); break;
        case 0x28: this->preOpcode->pre_opcode_28(); break;
        case 0x29: this->preOpcode->pre_opcode_29(); break;
        case 0x2A: this->preOpcode->pre_opcode_2A(); break;
        case 0x2B: this->preOpcode->pre_opcode_2B(); break;
        case 0x2C: this->preOpcode->pre_opcode_2C(); break;
        case 0x2D: this->preOpcode->pre_opcode_2D(); break;
        case 0x2E: this->preOpcode->pre_opcode_2E(); break;
        case 0x2F: this->preOpcode->pre_opcode_2F(); break;
        case 0x30: this->preOpcode->pre_opcode_30(); break;
        case 0x31: this->preOpcode->pre_opcode_31(); break;
        case 0x32: this->preOpcode->pre_opcode_32(); break;
        case 0x33: this->preOpcode->pre_opcode_33(); break;
        case 0x34: this->preOpcode->pre_opcode_34(); break;
        case 0x35: this->preOpcode->pre_opcode_35(); break;
        case 0x36: this->preOpcode->pre_opcode_36(); break;
        case 0x37: this->preOpcode->pre_opcode_37(); break;
        case 0x38: this->preOpcode->pre_opcode_38(); break;
        case 0x39: this->preOpcode->pre_opcode_39(); break;
        case 0x3A: this->preOpcode->pre_opcode_3A(); break;
        case 0x3B: this->preOpcode->pre_opcode_3B(); break;
        case 0x3C: this->preOpcode->pre_opcode_3C(); break;
        case 0x3D: this->preOpcode->pre_opcode_3D(); break;
        case 0x3E: this->preOpcode->pre_opcode_3E(); break;
        case 0x3F: this->preOpcode->pre_opcode_3F(); break;
        case 0x40: this->preOpcode->pre_opcode_40(); break;
        case 0x41: this->preOpcode->pre_opcode_41(); break;
        case 0x42: this->preOpcode->pre_opcode_42(); break;
        case 0x43: this->preOpcode->pre_opcode_43(); break;
        case 0x44: this->preOpcode->pre_opcode_44(); break;
        case 0x45: this->preOpcode->pre_opcode_45(); break;
        case 0x46: this->preOpcode->pre_opcode_46(); break;
        case 0x47: this->preOpcode->pre_opcode_47(); break;
        case 0x48: this->preOpcode->pre_opcode_48(); break;
        case 0x49: this->preOpcode->pre_opcode_49(); break;
        case 0x4A: this->preOpcode->pre_opcode_4A(); break;
        case 0x4B: this->preOpcode->pre_opcode_4B(); break;
        case 0x4C: this->preOpcode->pre_opcode_4C(); break;
        case 0x4D: this->preOpcode->pre_opcode_4D(); break;
        case 0x4E: this->preOpcode->pre_opcode_4E(); break;
        case 0x4F: this->preOpcode->pre_opcode_4F(); break;
        case 0x50: this->preOpcode->pre_opcode_50(); break;
        case 0x51: this->preOpcode->pre_opcode_51(); break;
        case 0x52: this->preOpcode->pre_opcode_52(); break;
        case 0x53: this->preOpcode->pre_opcode_53(); break;
        case 0x54: this->preOpcode->pre_opcode_54(); break;
        case 0x55: this->preOpcode->pre_opcode_55(); break;
        case 0x56: this->preOpcode->pre_opcode_56(); break;
        case 0x57: this->preOpcode->pre_opcode_57(); break;
        case 0x58: this->preOpcode->pre_opcode_58(); break;
        case 0x59: this->preOpcode->pre_opcode_59(); break;
        case 0x5A: this->preOpcode->pre_opcode_5A(); break;
        case 0x5B: this->preOpcode->pre_opcode_5B(); break;
        case 0x5C: this->preOpcode->pre_opcode_5C(); break;
        case 0x5D: this->preOpcode->pre_opcode_5D(); break;
        case 0x5E: this->preOpcode->pre_opcode_5E(); break;
        case 0x5F: this->preOpcode->pre_opcode_5F(); break;
        case 0x60: this->preOpcode->pre_opcode_60(); break;
        case 0x61: this->preOpcode->pre_opcode_61(); break;
        case 0x62: this->preOpcode->pre_opcode_62(); break;
        case 0x63: this->preOpcode->pre_opcode_63(); break;
        case 0x64: this->preOpcode->pre_opcode_64(); break;
        case 0x65: this->preOpcode->pre_opcode_65(); break;
        case 0x66: this->preOpcode->pre_opcode_66(); break;
        case 0x67: this->preOpcode->pre_opcode_67(); break;
        case 0x68: this->preOpcode->pre_opcode_68(); break;
        case 0x69: this->preOpcode->pre_opcode_69(); break;
        case 0x6A: this->preOpcode->pre_opcode_6A(); break;
        case 0x6B: this->preOpcode->pre_opcode_6B(); break;
        case 0x6C: this->preOpcode->pre_opcode_6C(); break;
        case 0x6D: this->preOpcode->pre_opcode_6D(); break;
        case 0x6E: this->preOpcode->pre_opcode_6E(); break;
        case 0x6F: this->preOpcode->pre_opcode_6F(); break;
        case 0x70: this->preOpcode->pre_opcode_70(); break;
        case 0x71: this->preOpcode->pre_opcode_71(); break;
        case 0x72: this->preOpcode->pre_opcode_72(); break;
        case 0x73: this->preOpcode->pre_opcode_73(); break;
        case 0x74: this->preOpcode->pre_opcode_74(); break;
        case 0x75: this->preOpcode->pre_opcode_75(); break;
        case 0x76: this->preOpcode->pre_opcode_76(); break;
        case 0x77: this->preOpcode->pre_opcode_77(); break;
        case 0x78: this->preOpcode->pre_opcode_78(); break;
        case 0x79: this->preOpcode->pre_opcode_79(); break;
        case 0x7A: this->preOpcode->pre_opcode_7A(); break;
        case 0x7B: this->preOpcode->pre_opcode_7B(); break;
        case 0x7C: this->preOpcode->pre_opcode_7C(); break;
        case 0x7D: this->preOpcode->pre_opcode_7D(); break;
        case 0x7E: this->preOpcode->pre_opcode_7E(); break;
        case 0x7F: this->preOpcode->pre_opcode_7F(); break;
        case 0x80: this->preOpcode->pre_opcode_80(); break;
        case 0x81: this->preOpcode->pre_opcode_81(); break;
        case 0x82: this->preOpcode->pre_opcode_82(); break;
        case 0x83: this->preOpcode->pre_opcode_83(); break;
        case 0x84: this->preOpcode->pre_opcode_84(); break;
        case 0x85: this->preOpcode->pre_opcode_85(); break;
        case 0x86: this->preOpcode->pre_opcode_86(); break;
        case 0x87: this->preOpcode->pre_opcode_87(); break;
        case 0x88: this->preOpcode->pre_opcode_88(); break;
        case 0x89: this->preOpcode->pre_opcode_89(); break;
        case 0x8A: this->preOpcode->pre_opcode_8A(); break;
        case 0x8B: this->preOpcode->pre_opcode_8B(); break;
        case 0x8C: this->preOpcode->pre_opcode_8C(); break;
        case 0x8D: this->preOpcode->pre_opcode_8D(); break;
        case 0x8E: this->preOpcode->pre_opcode_8E(); break;
        case 0x8F: this->preOpcode->pre_opcode_8F(); break;
        case 0x90: this->preOpcode->pre_opcode_90(); break;
        case 0x91: this->preOpcode->pre_opcode_91(); break;
        case 0x92: this->preOpcode->pre_opcode_92(); break;
        case 0x93: this->preOpcode->pre_opcode_93(); break;
        case 0x94: this->preOpcode->pre_opcode_94(); break;
        case 0x95: this->preOpcode->pre_opcode_95(); break;
        case 0x96: this->preOpcode->pre_opcode_96(); break;
        case 0x97: this->preOpcode->pre_opcode_97(); break;
        case 0x98: this->preOpcode->pre_opcode_98(); break;
        case 0x99: this->preOpcode->pre_opcode_99(); break;
        case 0x9A: this->preOpcode->pre_opcode_9A(); break;
        case 0x9B: this->preOpcode->pre_opcode_9B(); break;
        case 0x9C: this->preOpcode->pre_opcode_9C(); break;
        case 0x9D: this->preOpcode->pre_opcode_9D(); break;
        case 0x9E: this->preOpcode->pre_opcode_9E(); break;
        case 0x9F: this->preOpcode->pre_opcode_9F(); break;
        case 0xA0: this->preOpcode->pre_opcode_A0(); break;
        case 0xA1: this->preOpcode->pre_opcode_A1(); break;
        case 0xA2: this->preOpcode->pre_opcode_A2(); break;
        case 0xA3: this->preOpcode->pre_opcode_A3(); break;
        case 0xA4: this->preOpcode->pre_opcode_A4(); break;
        case 0xA5: this->preOpcode->pre_opcode_A5(); break;
        case 0xA6: this->preOpcode->pre_opcode_A6(); break;
        case 0xA7: this->preOpcode->pre_opcode_A7(); break;
        case 0xA8: this->preOpcode->pre_opcode_A8(); break;
        case 0xA9: this->preOpcode->pre_opcode_A9(); break;
        case 0xAA: this->preOpcode->pre_opcode_AA(); break;
        case 0xAB: this->preOpcode->pre_opcode_AB(); break;
        case 0xAC: this->preOpcode->pre_opcode_AC(); break;
        case 0xAD: this->preOpcode->pre_opcode_AD(); break;
        case 0xAE: this->preOpcode->pre_opcode_AE(); break;
        case 0xAF: this->preOpcode->pre_opcode_AF(); break;
        case 0xB0: this->preOpcode->pre_opcode_B0(); break;
        case 0xB1: this->preOpcode->pre_opcode_B1(); break;
        case 0xB2: this->preOpcode->pre_opcode_B2(); break;
        case 0xB3: this->preOpcode->pre_opcode_B3(); break;
        case 0xB4: this->preOpcode->pre_opcode_B4(); break;
        case 0xB5: this->preOpcode->pre_opcode_B5(); break;
        case 0xB6: this->preOpcode->pre_opcode_B6(); break;
        case 0xB7: this->preOpcode->pre_opcode_B7(); break;
        case 0xB8: this->preOpcode->pre_opcode_B8(); break;
        case 0xB9: this->preOpcode->pre_opcode_B9(); break;
        case 0xBA: this->preOpcode->pre_opcode_BA(); break;
        case 0xBB: this->preOpcode->pre_opcode_BB(); break;
        case 0xBC: this->preOpcode->pre_opcode_BC(); break;
        case 0xBD: this->preOpcode->pre_opcode_BD(); break;
        case 0xBE: this->preOpcode->pre_opcode_BE(); break;
        case 0xBF: this->preOpcode->pre_opcode_BF(); break;
        case 0xC0: this->preOpcode->pre_opcode_C0(); break;
        case 0xC1: this->preOpcode->pre_opcode_C1(); break;
        case 0xC2: this->preOpcode->pre_opcode_C2(); break;
        case 0xC3: this->preOpcode->pre_opcode_C3(); break;
        case 0xC4: this->preOpcode->pre_opcode_C4(); break;
        case 0xC5: this->preOpcode->pre_opcode_C5(); break;
        case 0xC6: this->preOpcode->pre_opcode_C6(); break;
        case 0xC7: this->preOpcode->pre_opcode_C7(); break;
        case 0xC8: this->preOpcode->pre_opcode_C8(); break;
        case 0xC9: this->preOpcode->pre_opcode_C9(); break;
        case 0xCA: this->preOpcode->pre_opcode_CA(); break;
        case 0xCB: this->preOpcode->pre_opcode_CB(); break;
        case 0xCC: this->preOpcode->pre_opcode_CC(); break;
        case 0xCD: this->preOpcode->pre_opcode_CD(); break;
        case 0xCE: this->preOpcode->pre_opcode_CE(); break;
        case 0xCF: this->preOpcode->pre_opcode_CF(); break;
        case 0xD0: this->preOpcode->pre_opcode_D0(); break;
        case 0xD1: this->preOpcode->pre_opcode_D1(); break;
        case 0xD2: this->preOpcode->pre_opcode_D2(); break;
        case 0xD3: this->preOpcode->pre_opcode_D3(); break;
        case 0xD4: this->preOpcode->pre_opcode_D4(); break;
        case 0xD5: this->preOpcode->pre_opcode_D5(); break;
        case 0xD6: this->preOpcode->pre_opcode_D6(); break;
        case 0xD7: this->preOpcode->pre_opcode_D7(); break;
        case 0xD8: this->preOpcode->pre_opcode_D8(); break;
        case 0xD9: this->preOpcode->pre_opcode_D9(); break;
        case 0xDA: this->preOpcode->pre_opcode_DA(); break;
        case 0xDB: this->preOpcode->pre_opcode_DB(); break;
        case 0xDC: this->preOpcode->pre_opcode_DC(); break;
        case 0xDD: this->preOpcode->pre_opcode_DD(); break;
        case 0xDE: this->preOpcode->pre_opcode_DE(); break;
        case 0xDF: this->preOpcode->pre_opcode_DF(); break;
        case 0xE0: this->preOpcode->pre_opcode_E0(); break;
        case 0xE1: this->preOpcode->pre_opcode_E1(); break;
        case 0xE2: this->preOpcode->pre_opcode_E2(); break;
        case 0xE3: this->preOpcode->pre_opcode_E3(); break;
        case 0xE4: this->preOpcode->pre_opcode_E4(); break;
        case 0xE5: this->preOpcode->pre_opcode_E5(); break;
        case 0xE6: this->preOpcode->pre_opcode_E6(); break;
        case 0xE7: this->preOpcode->pre_opcode_E7(); break;
        case 0xE8: this->preOpcode->pre_opcode_E8(); break;
        case 0xE9: this->preOpcode->pre_opcode_E9(); break;
        case 0xEA: this->preOpcode->pre_opcode_EA(); break;
        case 0xEB: this->preOpcode->pre_opcode_EB(); break;
        case 0xEC: this->preOpcode->pre_opcode_EC(); break;
        case 0xED: this->preOpcode->pre_opcode_ED(); break;
        case 0xEE: this->preOpcode->pre_opcode_EE(); break;
        case 0xEF: this->preOpcode->pre_opcode_EF(); break;
        case 0xF0: this->preOpcode->pre_opcode_F0(); break;
        case 0xF1: this->preOpcode->pre_opcode_F1(); break;
        case 0xF2: this->preOpcode->pre_opcode_F2(); break;
        case 0xF3: this->preOpcode->pre_opcode_F3(); break;
        case 0xF4: this->preOpcode->pre_opcode_F4(); break;
        case 0xF5: this->preOpcode->pre_opcode_F5(); break;
        case 0xF6: this->preOpcode->pre_opcode_F6(); break;
        case 0xF7: this->preOpcode->pre_opcode_F7(); break;
        case 0xF8: this->preOpcode->pre_opcode_F8(); break;
        case 0xF9: this->preOpcode->pre_opcode_F9(); break;
        case 0xFA: this->preOpcode->pre_opcode_FA(); break;
        case 0xFB: this->preOpcode->pre_opcode_FB(); break;
        case 0xFC: this->preOpcode->pre_opcode_FC(); break;
        case 0xFD: this->preOpcode->pre_opcode_FD(); break;
        case 0xFE: this->preOpcode->pre_opcode_FE(); break;
        case 0xFF: this->preOpcode->pre_opcode_FF(); break;
    }
}

// DI
void Opcode::opcode_F3()
{
    this->cpu->ime = 0;
}

// EI
void Opcode::opcode_FB()
{
    this->cpu->ime = 1;
    opcode = this->memory->read_memory(this->cpu->pc.PC++);
    this->system->counter += this->system->op_cycles[opcode];
    process();
}

void Opcode::process()
{
    switch (this->opcode) {
        case 0x00: break;
        case 0x01: Opcode::opcode_01(); break;
        case 0x02: Opcode::opcode_02(); break;
        case 0x03: Opcode::opcode_03(); break;
        case 0x04: Opcode::opcode_04(); break;
        case 0x05: Opcode::opcode_05(); break;
        case 0x06: Opcode::opcode_06(); break;
        case 0x07: Opcode::opcode_07(); break;
        case 0x08: Opcode::opcode_08(); break;
        case 0x09: Opcode::opcode_09(); break;
        case 0x0A: Opcode::opcode_0A(); break;
        case 0x0B: Opcode::opcode_0B(); break;
        case 0x0C: Opcode::opcode_0C(); break;
        case 0x0D: Opcode::opcode_0D(); break;
        case 0x0E: Opcode::opcode_0E(); break;
        case 0x0F: Opcode::opcode_0F(); break;
        case 0x10: Opcode::opcode_10(); break;
        case 0x11: Opcode::opcode_11(); break;
        case 0x12: Opcode::opcode_12(); break;
        case 0x13: Opcode::opcode_13(); break;
        case 0x14: Opcode::opcode_14(); break;
        case 0x15: Opcode::opcode_15(); break;
        case 0x16: Opcode::opcode_16(); break;
        case 0x17: Opcode::opcode_17(); break;
        case 0x18: Opcode::opcode_18(); break;
        case 0x19: Opcode::opcode_19(); break;
        case 0x1A: Opcode::opcode_1A(); break;
        case 0x1B: Opcode::opcode_1B(); break;
        case 0x1C: Opcode::opcode_1C(); break;
        case 0x1D: Opcode::opcode_1D(); break;
        case 0x1E: Opcode::opcode_1E(); break;
        case 0x1F: Opcode::opcode_1F(); break;
        case 0x20: Opcode::opcode_20(); break;
        case 0x21: Opcode::opcode_21(); break;
        case 0x22: Opcode::opcode_22(); break;
        case 0x23: Opcode::opcode_23(); break;
        case 0x24: Opcode::opcode_24(); break;
        case 0x25: Opcode::opcode_25(); break;
        case 0x26: Opcode::opcode_26(); break;
        case 0x27: Opcode::opcode_27(); break;
        case 0x28: Opcode::opcode_28(); break;
        case 0x29: Opcode::opcode_29(); break;
        case 0x2A: Opcode::opcode_2A(); break;
        case 0x2B: Opcode::opcode_2B(); break;
        case 0x2C: Opcode::opcode_2C(); break;
        case 0x2D: Opcode::opcode_2D(); break;
        case 0x2E: Opcode::opcode_2E(); break;
        case 0x2F: Opcode::opcode_2F(); break;
        case 0x30: Opcode::opcode_30(); break;
        case 0x31: Opcode::opcode_31(); break;
        case 0x32: Opcode::opcode_32(); break;
        case 0x33: Opcode::opcode_33(); break;
        case 0x34: Opcode::opcode_34(); break;
        case 0x35: Opcode::opcode_35(); break;
        case 0x36: Opcode::opcode_36(); break;
        case 0x37: Opcode::opcode_37(); break;
        case 0x38: Opcode::opcode_38(); break;
        case 0x39: Opcode::opcode_39(); break;
        case 0x3A: Opcode::opcode_3A(); break;
        case 0x3B: Opcode::opcode_3B(); break;
        case 0x3C: Opcode::opcode_3C(); break;
        case 0x3D: Opcode::opcode_3D(); break;
        case 0x3E: Opcode::opcode_3E(); break;
        case 0x3F: Opcode::opcode_3F(); break;
        case 0x40: Opcode::opcode_40(); break;
        case 0x41: Opcode::opcode_41(); break;
        case 0x42: Opcode::opcode_42(); break;
        case 0x43: Opcode::opcode_43(); break;
        case 0x44: Opcode::opcode_44(); break;
        case 0x45: Opcode::opcode_45(); break;
        case 0x46: Opcode::opcode_46(); break;
        case 0x47: Opcode::opcode_47(); break;
        case 0x48: Opcode::opcode_48(); break;
        case 0x49: Opcode::opcode_49(); break;
        case 0x4A: Opcode::opcode_4A(); break;
        case 0x4B: Opcode::opcode_4B(); break;
        case 0x4C: Opcode::opcode_4C(); break;
        case 0x4D: Opcode::opcode_4D(); break;
        case 0x4E: Opcode::opcode_4E(); break;
        case 0x4F: Opcode::opcode_4F(); break;
        case 0x50: Opcode::opcode_50(); break;
        case 0x51: Opcode::opcode_51(); break;
        case 0x52: Opcode::opcode_52(); break;
        case 0x53: Opcode::opcode_53(); break;
        case 0x54: Opcode::opcode_54(); break;
        case 0x55: Opcode::opcode_55(); break;
        case 0x56: Opcode::opcode_56(); break;
        case 0x57: Opcode::opcode_57(); break;
        case 0x58: Opcode::opcode_58(); break;
        case 0x59: Opcode::opcode_59(); break;
        case 0x5A: Opcode::opcode_5A(); break;
        case 0x5B: Opcode::opcode_5B(); break;
        case 0x5C: Opcode::opcode_5C(); break;
        case 0x5D: Opcode::opcode_5D(); break;
        case 0x5E: Opcode::opcode_5E(); break;
        case 0x5F: Opcode::opcode_5F(); break;
        case 0x60: Opcode::opcode_60(); break;
        case 0x61: Opcode::opcode_61(); break;
        case 0x62: Opcode::opcode_62(); break;
        case 0x63: Opcode::opcode_63(); break;
        case 0x64: Opcode::opcode_64(); break;
        case 0x65: Opcode::opcode_65(); break;
        case 0x66: Opcode::opcode_66(); break;
        case 0x67: Opcode::opcode_67(); break;
        case 0x68: Opcode::opcode_68(); break;
        case 0x69: Opcode::opcode_69(); break;
        case 0x6A: Opcode::opcode_6A(); break;
        case 0x6B: Opcode::opcode_6B(); break;
        case 0x6C: Opcode::opcode_6C(); break;
        case 0x6D: Opcode::opcode_6D(); break;
        case 0x6E: Opcode::opcode_6E(); break;
        case 0x6F: Opcode::opcode_6F(); break;
        case 0x70: Opcode::opcode_70(); break;
        case 0x71: Opcode::opcode_71(); break;
        case 0x72: Opcode::opcode_72(); break;
        case 0x73: Opcode::opcode_73(); break;
        case 0x74: Opcode::opcode_74(); break;
        case 0x75: Opcode::opcode_75(); break;
        case 0x76: Opcode::opcode_76(); break;
        case 0x77: Opcode::opcode_77(); break;
        case 0x78: Opcode::opcode_78(); break;
        case 0x79: Opcode::opcode_79(); break;
        case 0x7A: Opcode::opcode_7A(); break;
        case 0x7B: Opcode::opcode_7B(); break;
        case 0x7C: Opcode::opcode_7C(); break;
        case 0x7D: Opcode::opcode_7D(); break;
        case 0x7E: Opcode::opcode_7E(); break;
        case 0x7F: Opcode::opcode_7F(); break;
        case 0x80: Opcode::opcode_80(); break;
        case 0x81: Opcode::opcode_81(); break;
        case 0x82: Opcode::opcode_82(); break;
        case 0x83: Opcode::opcode_83(); break;
        case 0x84: Opcode::opcode_84(); break;
        case 0x85: Opcode::opcode_85(); break;
        case 0x86: Opcode::opcode_86(); break;
        case 0x87: Opcode::opcode_87(); break;
        case 0x88: Opcode::opcode_88(); break;
        case 0x89: Opcode::opcode_89(); break;
        case 0x8A: Opcode::opcode_8A(); break;
        case 0x8B: Opcode::opcode_8B(); break;
        case 0x8C: Opcode::opcode_8C(); break;
        case 0x8D: Opcode::opcode_8D(); break;
        case 0x8E: Opcode::opcode_8E(); break;
        case 0x8F: Opcode::opcode_8F(); break;
        case 0x90: Opcode::opcode_90(); break;
        case 0x91: Opcode::opcode_91(); break;
        case 0x92: Opcode::opcode_92(); break;
        case 0x93: Opcode::opcode_93(); break;
        case 0x94: Opcode::opcode_94(); break;
        case 0x95: Opcode::opcode_95(); break;
        case 0x96: Opcode::opcode_96(); break;
        case 0x97: Opcode::opcode_97(); break;
        case 0x98: Opcode::opcode_98(); break;
        case 0x99: Opcode::opcode_99(); break;
        case 0x9A: Opcode::opcode_9A(); break;
        case 0x9B: Opcode::opcode_9B(); break;
        case 0x9C: Opcode::opcode_9C(); break;
        case 0x9D: Opcode::opcode_9D(); break;
        case 0x9E: Opcode::opcode_9E(); break;
        case 0x9F: Opcode::opcode_9F(); break;
        case 0xA0: Opcode::opcode_A0(); break;
        case 0xA1: Opcode::opcode_A1(); break;
        case 0xA2: Opcode::opcode_A2(); break;
        case 0xA3: Opcode::opcode_A3(); break;
        case 0xA4: Opcode::opcode_A4(); break;
        case 0xA5: Opcode::opcode_A5(); break;
        case 0xA6: Opcode::opcode_A6(); break;
        case 0xA7: Opcode::opcode_A7(); break;
        case 0xA8: Opcode::opcode_A8(); break;
        case 0xA9: Opcode::opcode_A9(); break;
        case 0xAA: Opcode::opcode_AA(); break;
        case 0xAB: Opcode::opcode_AB(); break;
        case 0xAC: Opcode::opcode_AC(); break;
        case 0xAD: Opcode::opcode_AD(); break;
        case 0xAE: Opcode::opcode_AE(); break;
        case 0xAF: Opcode::opcode_AF(); break;
        case 0xB0: Opcode::opcode_B0(); break;
        case 0xB1: Opcode::opcode_B1(); break;
        case 0xB2: Opcode::opcode_B2(); break;
        case 0xB3: Opcode::opcode_B3(); break;
        case 0xB4: Opcode::opcode_B4(); break;
        case 0xB5: Opcode::opcode_B5(); break;
        case 0xB6: Opcode::opcode_B6(); break;
        case 0xB7: Opcode::opcode_B7(); break;
        case 0xB8: Opcode::opcode_B8(); break;
        case 0xB9: Opcode::opcode_B9(); break;
        case 0xBA: Opcode::opcode_BA(); break;
        case 0xBB: Opcode::opcode_BB(); break;
        case 0xBC: Opcode::opcode_BC(); break;
        case 0xBD: Opcode::opcode_BD(); break;
        case 0xBE: Opcode::opcode_BE(); break;
        case 0xBF: Opcode::opcode_BF(); break;
        case 0xC0: Opcode::opcode_C0(); break;
        case 0xC1: Opcode::opcode_C1(); break;
        case 0xC2: Opcode::opcode_C2(); break;
        case 0xC3: Opcode::opcode_C3(); break;
        case 0xC4: Opcode::opcode_C4(); break;
        case 0xC5: Opcode::opcode_C5(); break;
        case 0xC6: Opcode::opcode_C6(); break;
        case 0xC7: Opcode::opcode_C7(); break;
        case 0xC8: Opcode::opcode_C8(); break;
        case 0xC9: Opcode::opcode_C9(); break;
        case 0xCA: Opcode::opcode_CA(); break;
        case 0xCB: Opcode::opcode_CB(); break;
        case 0xCC: Opcode::opcode_CC(); break;
        case 0xCD: Opcode::opcode_CD(); break;
        case 0xCE: Opcode::opcode_CE(); break;
        case 0xCF: Opcode::opcode_CF(); break;
        case 0xD0: Opcode::opcode_D0(); break;
        case 0xD1: Opcode::opcode_D1(); break;
        case 0xD2: Opcode::opcode_D2(); break;
        case 0xD4: Opcode::opcode_D4(); break;
        case 0xD5: Opcode::opcode_D5(); break;
        case 0xD6: Opcode::opcode_D6(); break;
        case 0xD7: Opcode::opcode_D7(); break;
        case 0xD8: Opcode::opcode_D8(); break;
        case 0xD9: Opcode::opcode_D9(); break;
        case 0xDA: Opcode::opcode_DA(); break;
        case 0xDC: Opcode::opcode_DC(); break;
        case 0xDE: Opcode::opcode_DE(); break;
        case 0xDF: Opcode::opcode_DF(); break;
        case 0xE0: Opcode::opcode_E0(); break;
        case 0xE1: Opcode::opcode_E1(); break;
        case 0xE2: Opcode::opcode_E2(); break;
        case 0xE5: Opcode::opcode_E5(); break;
        case 0xE6: Opcode::opcode_E6(); break;
        case 0xE7: Opcode::opcode_E7(); break;
        case 0xE8: Opcode::opcode_E8(); break;
        case 0xE9: Opcode::opcode_E9(); break;
        case 0xEA: Opcode::opcode_EA(); break;
        case 0xEE: Opcode::opcode_EE(); break;
        case 0xEF: Opcode::opcode_EF(); break;
        case 0xF0: Opcode::opcode_F0(); break;
        case 0xF1: Opcode::opcode_F1(); break;
        case 0xF2: Opcode::opcode_F2(); break;
        case 0xF3: Opcode::opcode_F3(); break;
        case 0xF5: Opcode::opcode_F5(); break;
        case 0xF6: Opcode::opcode_F6(); break;
        case 0xF7: Opcode::opcode_F7(); break;
        case 0xF8: Opcode::opcode_F8(); break;
        case 0xF9: Opcode::opcode_F9(); break;
        case 0xFA: Opcode::opcode_FA(); break;
        case 0xFB: Opcode::opcode_FB(); break;
        case 0xFE: Opcode::opcode_FE(); break;
        case 0xFF: Opcode::opcode_FF(); break;
    }
}
