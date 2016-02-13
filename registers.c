#include "cpu.h"
#include "registers.h"

union registers regs;
union stack_pointer sp;
union program_counter pc;

BYTE * const A = &regs.byte.A;
BYTE * const F = &regs.byte.F;
BYTE * const B = &regs.byte.B;
BYTE * const C = &regs.byte.C;
BYTE * const D = &regs.byte.D;
BYTE * const E = &regs.byte.E;
BYTE * const H = &regs.byte.H;
BYTE * const L = &regs.byte.L;
WORD * const AF = &regs.word.AF;
WORD * const BC = &regs.word.BC;
WORD * const DE = &regs.word.DE;
WORD * const HL = &regs.word.HL;
