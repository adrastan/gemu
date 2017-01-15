<<<<<<< HEAD
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
void update_lcd(int);
int get_mode(void);
void inc_ly(void);
void process_opcode(void);
int is_set(u_int8,int);
void switch_mode(int);
void compare_ly(void);

#endif // CPU_H
=======
#ifndef CPU_H
#define CPU_H

typedef unsigned char BYTE;
typedef unsigned short WORD;

#define SET_Z(F) F |= 1 << 7
#define RESET_Z(F) F &= ~(1 << 7)
#define SET_N(F) F |= 1 << 6
#define RESET_N(F) F &= ~(1 << 6)
#define SET_H(F) F |= 1 << 5
#define RESET_H(F) F &= ~(1 << 5)
#define SET_C(F) F |= 1 << 4
#define RESET_C(F) F &= ~(1 << 4)

void start_cpu(void);
BYTE test_z(BYTE);
BYTE test_n(BYTE);
BYTE test_c(BYTE);
BYTE test_h(BYTE);

#endif // CPU_H
>>>>>>> origin/master
