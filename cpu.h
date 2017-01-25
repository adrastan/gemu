#ifndef CPU_H
#define CPU_H

#include <stdio.h>

#include "cpu_types.h"

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

#endif // CPU_H
