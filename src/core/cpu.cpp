#include "cpu.h"

Cpu::Cpu()
{
    pc.PC = 0x0100;
    sp.SP = 0xFFFE;
    regs.word.AF = 0x01B0;
    regs.word.BC = 0x0013;
    regs.word.DE = 0x00D8;
    regs.word.HL = 0x014D;
}

// returns 1 if flag z is set
u8 Cpu::test_z()
{
    return (regs.byte.F & 0x80) >> 7;
}

// set z flag
void Cpu::set_z()
{
    regs.byte.F |= 1 << 7;
}

// reset z flag
void Cpu::reset_z()
{
    regs.byte.F &= ~(1 << 7);
}

// returns 1 if flag n is set
u8 Cpu::test_n()
{
    return (regs.byte.F & 0x40) >> 6;
}

// set n flag
void Cpu::set_n()
{
    regs.byte.F |= 1 << 6;
}

// reset n flag
void Cpu::reset_n()
{
    regs.byte.F &= ~(1 << 6);
}

// returns 1 if flag h is set
u8 Cpu::test_h()
{
    return (regs.byte.F & 0x20) >> 5;
}

// set h flag
void Cpu::set_h()
{
    regs.byte.F |= 1 << 5;
}

// reset h flag
void Cpu::reset_h()
{
    regs.byte.F &= ~(1 << 5);
}

// returns 1 if flag c is set
u8 Cpu::test_c()
{
    return (regs.byte.F & 0x10) >> 4;
}

// set c flag
void Cpu::set_c()
{
    regs.byte.F |= 1 << 4;
}

// reset c flag
void Cpu::reset_c()
{
    regs.byte.F &= ~(1 << 4);
}
