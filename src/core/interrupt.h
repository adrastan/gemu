
#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "memory.h"
#include "cpu.h"

class Memory;
class Cpu;

class Interrupt
{
    public:
    Memory *memory;
    Cpu *cpu;

    void check_interrupts();
    void do_interrupts();
    void request_interrupt(int);
    void discard_interrupt(int);
};

#endif // INTERRUPT_H
