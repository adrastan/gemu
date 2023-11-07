/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#ifndef CPU_H
#define CPU_H

#include <stdio.h>

#include <SDL.h>
#include "cpu_types.h"

extern SDL_Window* sdl_window;
extern SDL_Surface* screen_surface;
extern SDL_Texture* texture;
extern SDL_Renderer* renderer;
void update_frame(SDL_Event *);

#define SCREEN_WIDTH 160*3
#define SCREEN_HEIGHT 144*3

extern int save_request;
extern int load_request;
extern int cap;
extern int ime;
extern int halt;
extern int stopped;
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
extern int paused;
extern int next;

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
int process_opcode(void);
int is_set(u_int16,int);
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

void prepare_file(char *);

#endif // CPU_H
