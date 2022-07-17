/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include "memory.h"
#include "timers.h"
#include "cpu.h"
#include "interrupts.h"

int timer_control;
int timer_cycles;
int divider = 256;

void update_clock()
{
    if (memory[0xff05] == 0xff) {
        memory[0xff05] = memory[0xff06];
        request_interrupt(2);
    } else {
        ++memory[0xff05];
    }
    if (timer_control == 0) {
        timer_cycles += 1024;
    } else if (timer_control == 1) {
        timer_cycles += 16;
    } else if (timer_control == 2) {
        timer_cycles += 64;
    } else {
        timer_cycles += 256;
    }
}

void update_divider(int cycles)
{
    divider -= cycles;
    if (divider <= 0) {
        ++memory[0xff04];
        divider += 256;
    }
}

void update_timers(int cycles)
{
    update_divider(cycles);
    int cur_control = memory[0xff07] & 0x03;

    // clock is disabled
    if (!is_set(memory[0xff07], 2)) {
        return;
    }

    // if control has not changed since last instruction
    if (timer_control == cur_control) {
        timer_cycles -= cycles;
        if (timer_cycles <= -4) {
            update_clock();
        }
    } else {
        timer_control = cur_control;
        memory[0xff05] = 0;
        set_clock_freq();
    }
}

void set_clock_freq()
{
    int control = memory[0xff07] & 3;

    switch (control) {
        case 0: timer_cycles = 1024;
        case 1: timer_cycles = 16;
        case 2: timer_cycles = 64;
        case 3: timer_cycles = 256;
    }
}
