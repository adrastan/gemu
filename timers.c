#include "memory.h"
#include "timers.h"
#include "cpu.h"
#include "interrupts.h"

int timer_control;
int timer_freq;
int divider = 16384;

void update_clock()
{
    if (memory[0xff05] == 0xff) {
        memory[0xff05] = memory[0xff06];
        request_interrupt(2);
    } else {
        ++memory[0xff05];
    }
}

void update_divider(int cycles)
{
    divider += cycles;
    if (divider <= 0) {
        ++memory[0xff04];
        divider += 16384;
    }
}

void update_timers(int cycles)
{
    update_divider(cycles);
    int cur_control = memory[0xff07] & 0x03;

    // check if clock is enabled
    if (is_set(memory[0xff07],2)) {
        // if control has not changed since last instruction
        if (timer_control == cur_control) {
            timer_freq += cycles;
            if (timer_control == 0) {
                if (timer_freq <= 0) {
                    update_clock();
                    timer_freq += 4096;
                }
            } else if (timer_control == 1) {
                if (timer_freq <= 0) {
                    update_clock();
                    timer_freq += 262144;
                }
            } else if (timer_control == 2) {
                if (timer_freq <= 0) {
                    update_clock();
                    timer_freq += 65536;
                }
            } else {
                if (timer_freq <= 0) {
                    update_clock();
                    timer_freq += 16384;
                }
            }
        } else {
            timer_control = cur_control;
            memory[0xff05] = 0;
            set_clock_freq();
        }
    }
}

void set_clock_freq()
{
    int control = memory[0xff07] & 3;

    switch (control) {
        case 0: timer_freq = 4096;
        case 1: timer_freq = 262144;
        case 2: timer_freq = 65536;
        case 3: timer_freq = 16384;
    }
}
