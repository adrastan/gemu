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