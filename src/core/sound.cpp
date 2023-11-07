/*
 * Gemu - Nintendo Game Boy Emulator
 * Copyright (C) 2017  Alex Dempsey
 */

#include <stdlib.h>
#include <stdio.h>

#include <Basic_Gb_Apu.h>
#include <gb_apu/Multi_Buffer.h>
#include <Sound_Queue.h>

#include "sound.h"
#include "memory.h"

int is_enabled() {
	return is_set(memory[0xFF26], 7);
}

void update_sound_control(u_int8 byte) {
	if (byte == 0) {
		memory[0xFF26] = 0x70;
		clear_sound_regs();
	} else if (is_set(byte, 7)) {
		memory[0xFF26] = 0xF0;
	}
}

void update_apu(u_int16 address, u_int8 byte) {
	// wave ram
    if (address >= 0xFF30 && address <= 0xFF3F) {
        memory[address] = byte;
		return;
    }

	if (address == 0xFF26) {
		update_sound_control(byte);
	} else if (is_enabled()) {
		memory[address] = byte;
	}
}

static Basic_Gb_Apu apu;
static Sound_Queue sound;


static void handle_error( const char* str )
{
	if ( str )
	{
		fprintf( stderr, "Error: %s\n", str );
		exit( EXIT_FAILURE );
	}
}

void init_sound()
{
	long const sample_rate = 44100;
	handle_error( apu.set_sample_rate( sample_rate ) );
	
	// Generate a few seconds of sound and play using SDL
	handle_error( sound.start( sample_rate, 2 ) );
}


void sync_sound(u_int16 address, u_int8 byte)
{
	update_apu(address, byte);
	apu.write_register( address, byte);
}

void update_sound(int counter)
{
	apu.end_frame();
	int const buf_size = 2048;
	static blip_sample_t buf [buf_size];
		
	// Play whatever samples are available
	long count = apu.read_samples( buf, buf_size );
	sound.write( buf, count );
}

