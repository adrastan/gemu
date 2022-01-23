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

#ifndef REGISTERS_H
#define REGISTERS_H

#include "cpu_types.h"

// main registers
union registers {
    struct {
        u_int16 AF, BC, DE, HL; // MSB -> LSB
    } word;
    struct {
        u_int8 F, A, C, B, E, D, L, H; // LSB -> MSB
    } byte;
};

// stack pointer
union stack_pointer {
    u_int16 SP;
    struct {
        u_int8 L, H;
    } byte;
};

// program counter
union program_counter {
    u_int16 PC;
    struct {
        u_int8 L, H;
    } byte;
};

extern union registers regs;
extern union stack_pointer sp;
extern union program_counter pc;

#endif // REGISTERS_H
