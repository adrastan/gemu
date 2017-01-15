#include "interrupts.h"
#include "memory.h"
#include "cpu.h"
#include "registers.h"

extern int ime;
extern int counter;

void check_interrupts()
{
    u_int8 i_enable = read_memory(0xffff); // get interrupt enable register
    u_int8 i_request = read_memory(0xff0f); // get interrupt request register

    // if interrupt master enable is set
    if (ime) {
        // v blank
        if ((is_set(i_enable,0)) && (is_set(i_request,0))) {
            ime = 0; // reset master enable
            write_memory(0xff0f, read_memory(0xff0f) & ~(1 << 0)); // reset v blank request
            write_memory(sp.SP-1, pc.byte.H);
            write_memory(sp.SP-2, pc.byte.L);
            sp.SP -= 2;
            pc.PC = 0x0040;
            counter -= 20;
        // lcd stat
        } else if ((is_set(i_enable,1)) && (is_set(i_request,1))) {
            ime = 0; // reset master enable
            write_memory(0xff0f, read_memory(0xff0f) & ~(1 << 1)); // reset lcd stat request
            write_memory(sp.SP-1, pc.byte.H);
            write_memory(sp.SP-2, pc.byte.L);
            sp.SP -= 2;
            pc.PC = 0x0048;
            counter -= 20;
        // timer
        } else if ((is_set(i_enable,2)) && (is_set(i_request,2))) {
            ime = 0; // reset master enable
            write_memory(0xff0f, read_memory(0xff0f) & ~(1 << 2)); // reset timer request
            write_memory(sp.SP-1, pc.byte.H);
            write_memory(sp.SP-2, pc.byte.L);
            sp.SP -= 2;
            pc.PC = 0x0050;
            counter -= 20;
        // serial
        } else if ((is_set(i_enable,3)) && (is_set(i_request,3))) {
            ime = 0; // reset master enable
            write_memory(0xff0f, read_memory(0xff0f) & ~(1 << 3)); // reset serial request
            write_memory(sp.SP-1, pc.byte.H);
            write_memory(sp.SP-2, pc.byte.L);
            sp.SP -= 2;
            pc.PC = 0x0058;
            counter -= 20;
        // joypad
        } else if ((is_set(i_enable,4)) && (is_set(i_request,4))) {
            ime = 0; // reset master enable
            write_memory(0xff0f, read_memory(0xff0f) & ~(1 << 4)); // reset serial request
            write_memory(sp.SP-1, pc.byte.H);
            write_memory(sp.SP-2, pc.byte.L);
            sp.SP -= 2;
            pc.PC = 0x0060;
            counter -= 20;
        }
    }
}

void request_interrupt(int interrupt)
{
    switch (interrupt) {
        case 0: memory[0xff0f] |= (1 << 0); break;
        case 1: memory[0xff0f] |= (1 << 1); break;
        case 2: memory[0xff0f] |= (1 << 2); break;
        case 3: memory[0xff0f] |= (1 << 3); break;
        case 4: memory[0xff0f] |= (1 << 4); break;
    }
}
