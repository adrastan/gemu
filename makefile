INC=-I./ \
	-I./Gb_Snd_Emu-0.1.4 \
	-I./Gb_Snd_Emu-0.1.4/gb_apu \
	
SDL_INC=-IC:\msys64\mingw64\include \
		-IC:\msys64\mingw64\include\SDL2

LIB=-L./

C_OBJS=obj/cpu.o obj/interrupts.o obj/joypad.o \
	obj/lcd_controller.o obj/memory.o obj/opcodes.o obj/pre_opcodes.o obj/registers.o \
	obj/timers.o

C++_OBJS=obj/main.o obj/sound.o obj/Basic_Gb_Apu.o obj/Sound_Queue.o obj/Multi_Buffer.o obj/Gb_Oscs.o \
	obj/Gb_Apu.o obj/Blip_Buffer.o

EMC_OBJS=emc/obj/sound.o emc/obj/cpu.o emc/obj/interrupts.o emc/obj/joypad.o \
	emc/obj/lcd_controller.o emc/obj/memory.o emc/obj/opcodes.o emc/obj/pre_opcodes.o emc/obj/registers.o \
	emc/obj/timers.o emc/obj/emc.o

C_FLAGS=-Wall -O2

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

web	: clean $(EMC_OBJS)
	emcc -o emc/gemu.html --shell-file emc/shell.html $(EMC_OBJS) --preload-file games

emc/obj/%.o : src/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

emc/obj/%.o : Gb_Snd_Emu-0.1.4/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

emc/obj/%.o : Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

emc/obj/%.o : src/%.cpp
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@

emc/obj/%.o : src/%.c
	emcc $(INC) $(SDL_INC) -O2 -c $< -o $@


pc : clean main.exe

main.exe : $(C_OBJS) $(C++_OBJS)
	g++ -o main.exe $(C_OBJS) $(C++_OBJS) $(C_FLAGS) $(LIB) $(SDL_LDFLAGS)

obj/%.o : src/%.cpp
	g++ $(INC) $(SDL_CFLAGS) -O2 -c $< -o $@

obj/%.o : Gb_Snd_Emu-0.1.4/%.cpp
	g++ $(INC) $(SDL_CFLAGS) -O2 -c $< -o $@

obj/%.o : Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	g++ $(INC) $(SDL_CFLAGS) -O2 -c $< -o $@	

obj/%.o : src/%.cpp
	g++ $(INC) $(SDL_CFLAGS) -O2 -c $< -o $@	

obj/%.o : src/%.c
	gcc $(INC) $(SDL_CFLAGS) -O2 -c $< -o $@

clean :
	rm -rf main.exe obj/* emc/obj/*