#include "interrupts.h"
#include "memory.h"
#include "cpu.h"
#include "registers.h"

extern int ime;
extern int counter;
extern int halt;

void check_interrupts()
{
    u_int8 i_enable = read_memory(0xffff); // get interrupt enable register
    u_int8 i_request = read_memory(0xff0f); // get interrupt request register

    if (halt) {
        if ((is_set(i_enable,0)) && (is_set(i_request,0))) {
            halt = 0;
        } else if ((is_set(i_enable,1)) && (is_set(i_request,1))) {
            halt = 0;
        } else if ((is_set(i_enable,2)) && (is_set(i_request,2))) {
            halt = 0;
        } else if ((is_set(i_enable,3)) && (is_set(i_request,3))) {
            halt = 0;
        } else if ((is_set(i_enable,4)) && (is_set(i_request,4))) {
            halt = 0;
        }
    }

    // if interrupt master enable is set
    if (ime) {
        // v blank
        if ((is_set(i_enable,0)) && (is_set(i_request,0))) {
            ime = 0; // reset master enable
            discard_interrupt(0);
            push(pc.PC);
            pc.PC = 0x0040;
            set_delay(20);
        // lcd stat
        } else if ((is_set(i_enable,1)) && (is_set(i_request,1))) {
            ime = 0; // reset master enable
            discard_interrupt(1);
            push(pc.PC);
            pc.PC = 0x0048;
            set_delay(20);
        // timer
        } else if ((is_set(i_enable,2)) && (is_set(i_request,2))) {
            ime = 0; // reset master enable
            discard_interrupt(2);
            push(pc.PC);
            pc.PC = 0x0050;
            set_delay(20);
        // serial
        } else if ((is_set(i_enable,3)) && (is_set(i_request,3))) {
            ime = 0; // reset master enable
            discard_interrupt(3);
            push(pc.PC);
            pc.PC = 0x0058;
            set_delay(20);
        // joypad
        } else if ((is_set(i_enable,4)) && (is_set(i_request,4))) {
            ime = 0; // reset master enable
            discard_interrupt(4);
            push(pc.PC);
            pc.PC = 0x0060;
            set_delay(20);
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

void discard_interrupt(int interrupt)
{
    switch (interrupt) {
        case 0: memory[0xff0f] &= ~(1 << 0); break;
        case 1: memory[0xff0f] &= ~(1 << 1); break;
        case 2: memory[0xff0f] &= ~(1 << 2); break;
        case 3: memory[0xff0f] &= ~(1 << 3); break;
        case 4: memory[0xff0f] &= ~(1 << 4); break;
    }
}
