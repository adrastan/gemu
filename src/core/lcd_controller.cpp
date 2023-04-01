#include "lcd_controller.h"
#include "bit_helper.h"

LCDController::LCDController(Memory * const memory)
{
    this->memory = memory;
}

void LCDController::draw_frame()
{

}

void LCDController::update(int cycles)
{
    // reset lcd values if screen is disabled
    if (!(BitHelper::is_set(this->memory->read_memory(0xff40), 7))) {
        interrupt_cycles[3] = TOTAL;
        interrupt_cycles[0] = MODE_0;
        interrupt_cycles[1] = MODE_2;
        interrupt_cycles[2] = MODE_3;
        this->memory->write_memory(0xff44, 0);
        this->memory->write_memory(0xff41, this->memory->read_memory(0xff41) & 0xf8);
        return;
    }

    int mode = this->get_mode();

    // currently in mode 2
    if (mode == 2) {
        interrupt_cycles[1] -= cycles;
        if (interrupt_cycles[1] <= 0) {
            interrupt_cycles[2] += interrupt_cycles[1];
            interrupt_cycles[1] = MODE_2;
            switch_mode(3);
        }
    // currently in mode 3
    } else if (mode == 3) {
        interrupt_cycles[2] -= cycles;
        if (interrupt_cycles[2] <= 0) {
            interrupt_cycles[0] += interrupt_cycles[2];
            interrupt_cycles[2] = MODE_3;
            switch_mode(0);
            if (memory[0xff44] < 144) {
                update_graphics();
                if (dma.active) {
                    do_hdma();
                }
            }
            if (is_set(memory[0xff41],3)) {
                request_interrupt(1);
            }
        }
    // currently in mode 0
    } else if (mode == 0) {
        interrupt_cycles[0] -= cycles;
        if (interrupt_cycles[0] <= 0) {
            if (memory[0xff44] < 144) {
                interrupt_cycles[1] += interrupt_cycles[0];
                interrupt_cycles[0] = MODE_0;
                switch_mode(2);
                if (is_set(memory[0xff41],5)) {
                    request_interrupt(1);
                }
            }
        }
    // currently in vblank ie mode 1
    } else {
        if (memory[0xff44] == 0) {
            switch_mode(2);
            if (is_set(memory[0xff41],5)) {
                request_interrupt(1);
            }
        }
    }

    interrupt_cycles[3] -= cycles;

    // increments ly every cycle through all modes
    if (interrupt_cycles[3] <= 0) {
        interrupt_cycles[3] += TOTAL;
        compare_ly();
        inc_ly();
    }
    // last scanline so switch to mode 1 and generate
    // vblank interrupt
    if (memory[0xff44] == 144 && mode != 1) {
        interrupt_cycles[0] = MODE_0;
        switch_mode(1);
        if (save_request) {
            save_state();
        }
        if (load_request) {
            load_state();
        }
        request_interrupt(0);
        if (is_set(memory[0xff41],4)) {
            request_interrupt(1);
        }
    }
}

int LCDController::get_mode()
{
    return this->memory->read_memory(0xff41) & 0x03;
}

void LCDController::switch_mode(int)
{

}
