/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#ifndef TIMERS_H
#define TIMERS_H

void update_timers(int);
void update_clock(void);
void update_divider(int);
void set_clock_freq(void);

#endif // TIMERS_H
