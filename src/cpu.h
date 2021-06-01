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

#ifndef CPU_H
#define CPU_H

#include <stdio.h>

#ifndef EMSCRIPTEN
#include <SDL2/SDL.h>
void update_frame(SDL_Event *);
#endif

#ifdef EMSCRIPTEN
void update_frame(void);
#endif

#include "cpu_types.h"

extern int debug;
extern int save_request;
extern int load_request;
extern int cap;
extern int ime;
extern int halt;
extern int counter;
extern char * file_name;
extern char * title;
extern char * save_file;
extern int pre_cycles[];
extern int op_cycles[];
extern u_int8 opcode;
extern char * title;
extern char * save_file;
extern char * file_location;

void prepare_file(char*);
void start_cpu(char *);
void start_main_loop(void);
void get_next_frame(void);
u_int8 test_z(void);
u_int8 test_n(void);
u_int8 test_c(void);
u_int8 test_h(void);
void set_z(void);
void reset_z(void);
void set_n(void);
void reset_n(void);
void set_h(void);
void reset_h(void);
void set_c(void);
void reset_c(void);
void init_regs(void);
void read_rom(FILE *);
int half_carry(u_int8,u_int8);
void process_opcode(void);
int is_set(u_int8,int);
void push(u_int16);
void set_delay(int);
void print_flags(void);
void set_bit(u_int8*,int);
void reset_bit(u_int8*,int);
void update_serial(int);
void save_state(void);
void load_state(void);
u_int8* get_state(void);
void restart(void);
void initCart(void);

#endif // CPU_H
