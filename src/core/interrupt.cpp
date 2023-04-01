#include "interrupt.h"
#include "system_types.h"
#include <bit_helper.h>

void Interrupt::check_interrupts()
{
    u8 i_enable = this->memory->read_memory(0xffff); // get interrupt enable register
    u8 i_request = this->memory->read_memory(0xff0f); // get interrupt request register

    if (this->cpu->halt && (BitHelper::is_set(i_enable,0)) && (BitHelper::is_set(i_request,0))) {
        this->cpu->halt = 0;
    } else if (this->cpu->halt && (BitHelper::is_set(i_enable,1)) && (BitHelper::is_set(i_request,1))) {
        this->cpu->halt = 0;
    } else if (this->cpu->halt && (BitHelper::is_set(i_enable,2)) && (BitHelper::is_set(i_request,2))) {
        this->cpu->halt = 0;
    } else if (this->cpu->halt && (BitHelper::is_set(i_enable,3)) && (BitHelper::is_set(i_request,3))) {
        this->cpu->halt = 0;
    } else if (this->cpu->halt && (BitHelper::is_set(i_enable,4)) && (BitHelper::is_set(i_request,4))) {
        this->cpu->halt = 0;
    }

    // if interrupt master enable is set
    if (this->cpu->ime) {
        do_interrupts();
    }
}

void Interrupt::do_interrupts()
{
    u8 i_enable = this->memory->read_memory(0xffff); // get interrupt enable register
    u8 i_request = this->memory->read_memory(0xff0f); // get interrupt request register

    // v blank
    if ((BitHelper::is_set(i_enable,0)) && (BitHelper::is_set(i_request,0))) {
        this->cpu->ime = 0; // reset master enable
        discard_interrupt(0);
        this->cpu->push(this->cpu->pc.PC);
        this->cpu->pc.PC = 0x0040;
        this->cpu->delay = 20;
        // lcd stat
    } else if ((BitHelper::is_set(i_enable,1)) && (BitHelper::is_set(i_request,1))) {
        this->cpu->ime = 0; // reset master enable
        discard_interrupt(1);
        this->cpu->push(this->cpu->pc.PC);
        this->cpu->pc.PC = 0x0048;
        this->cpu->delay = 20;
        // tthis->cpu->imer
    } else if ((BitHelper::is_set(i_enable,2)) && (BitHelper::is_set(i_request,2))) {
        this->cpu->ime = 0; // reset master enable
        discard_interrupt(2);
        this->cpu->push(this->cpu->pc.PC);
        this->cpu->pc.PC = 0x0050;
        this->cpu->delay = 20;
        // serial
    } else if ((BitHelper::is_set(i_enable,3)) && (BitHelper::is_set(i_request,3))) {
        this->cpu->ime = 0; // reset master enable
        discard_interrupt(3);
        this->cpu->push(this->cpu->pc.PC);
        this->cpu->pc.PC = 0x0058;
        this->cpu->delay = 20;
        // joypad
    } else if ((BitHelper::is_set(i_enable,4)) && (BitHelper::is_set(i_request,4))) {
        this->cpu->ime = 0; // reset master enable
        discard_interrupt(4);
        this->cpu->push(this->cpu->pc.PC);
        this->cpu->pc.PC = 0x0060;
        this->cpu->delay = 20;
    }
}

void Interrupt::request_interrupt(int interrupt)
{
    switch (interrupt) {
        case 0: BitHelper::set_bit(&this->memory->memory[0xff0f],0); break;
        case 1: BitHelper::set_bit(&this->memory->memory[0xff0f],1); break;
        case 2: BitHelper::set_bit(&this->memory->memory[0xff0f],2); break;
        case 3: BitHelper::set_bit(&this->memory->memory[0xff0f],3); break;
        case 4: BitHelper::set_bit(&this->memory->memory[0xff0f],4); break;
    }
}

void Interrupt::discard_interrupt(int interrupt)
{
    switch (interrupt) {
        case 0: BitHelper::reset_bit(&this->memory->memory[0xff0f],0); break;
        case 1: BitHelper::reset_bit(&this->memory->memory[0xff0f],1); break;
        case 2: BitHelper::reset_bit(&this->memory->memory[0xff0f],2); break;
        case 3: BitHelper::reset_bit(&this->memory->memory[0xff0f],3); break;
        case 4: BitHelper::reset_bit(&this->memory->memory[0xff0f],4); break;
    }
}
