INC=-I./ \
	-I./Gb_Snd_Emu-0.1.4 \
	-I./Gb_Snd_Emu-0.1.4/gb_apu \
	-I./src/core
	
SDL_INC=-IC:\msys64\mingw64\include \
		-IC:\msys64\mingw64\include\SDL2

LIB=-L./

C_OBJS=obj/cpu.o obj/interrupts.o obj/joypad.o \
	obj/lcd_controller.o obj/memory.o obj/opcodes.o obj/pre_opcodes.o obj/registers.o \
	obj/timers.o

C++_OBJS=obj/main.o obj/sound.o obj/Basic_Gb_Apu.o obj/Sound_Queue.o obj/Multi_Buffer.o obj/Gb_Oscs.o \
	obj/Gb_Apu.o obj/Blip_Buffer.o

EMC_OBJS=build/emc/obj/sound.o build/emc/obj/cpu.o build/emc/obj/interrupts.o build/emc/obj/joypad.o \
        build/emc/obj/lcd_controller.o build/emc/obj/memory.o build/emc/obj/opcodes.o build/emc/obj/pre_opcodes.o build/emc/obj/registers.o \
        build/emc/obj/timers.o build/emc/obj/emc.o

C_FLAGS=-Wall -O2

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

web	: clean $(EMC_OBJS)
	emcc -o build/emc/gemu.html --shell-file src/web/shell.html $(EMC_OBJS)

build/emc/obj/%.o : src/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

build/emc/obj/%.o : src/external/Gb_Snd_Emu-0.1.4/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

build/emc/obj/%.o : src/external/Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

build/emc/obj/%.o : src/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

build/emc/obj/%.o : src/%.c
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

build/emc/obj/%.o : src/core/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

clean :
	rm -rf build/emc/main.exe build/emc/obj/* build/emc/obj/*
