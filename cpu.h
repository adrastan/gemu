#ifndef CPU_H
#define CPU_H

#include <stdio.h>

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

void start_cpu(void);
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

#endif // CPU_H
