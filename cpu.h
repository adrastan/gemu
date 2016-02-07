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
