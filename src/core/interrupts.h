/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void check_interrupts(void);
void request_interrupt(int);
void discard_interrupt(int);
void do_interrupts(void);

#endif // INTERRUPTS_H


