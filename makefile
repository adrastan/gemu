INC=-I./SDL2-2.0.14/i686-w64-mingw32/include/SDL2 \
	-I./SDL2-2.0.14/i686-w64-mingw32/include \
	-I./Gb_Snd_Emu-0.1.4/gb_apu \
	-I./Gb_Snd_Emu-0.1.4 \
	-IC:/mingw-w64/mingw64/include

LIB=-L./ \
	-L./SDL2-2.0.14/x86_64-w64-mingw32/lib

C_OBJS=obj/cpu.o obj/interrupts.o obj/joypad.o \
	obj/lcd_controller.o obj/memory.o obj/opcodes.o obj/pre_opcodes.o obj/registers.o \
	obj/timers.o

C++_OBJS=obj/main.o obj/sound.o obj/Basic_Gb_Apu.o obj/Sound_Queue.o obj/Multi_Buffer.o obj/Gb_Oscs.o \
	obj/Gb_Apu.o obj/Blip_Buffer.o

EMC_OBJS=emc/obj/cpu.o emc/obj/interrupts.o emc/obj/joypad.o \
	emc/obj/lcd_controller.o emc/obj/memory.o emc/obj/opcodes.o \
	emc/obj/pre_opcodes.o emc/obj/registers.o \
	emc/obj/timers.o emc/obj/main.o emc/obj/sound.o emc/obj/Basic_Gb_Apu.o \
	emc/obj/Sound_Queue.o emc/obj/Multi_Buffer.o emc/obj/Gb_Oscs.o \
	emc/obj/Gb_Apu.o emc/obj/Blip_Buffer.o

#SDL2_FLAGS=-lSDL2main -lSDL2 -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 \
#    -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

C_FLAGS=-Wall -std=c++11 -lmingw32 -lSDL2 -lSDL2main -O2 -pthread

emc	: $(EMC_OBJS)
	emcc -o emc/gemu.html $(EMC_OBJS) -s USE_SDL=2 --preload-file games/tetris.gb -pthread

emc/obj/%.o : src/%.cpp
	emcc $(INC) -c $< -o $@ -pthread

# obj/%.o : src/Windows/%.cpp
# 	g++ $(INC) -c $< -o $@

emc/obj/%.o : Gb_Snd_Emu-0.1.4/%.cpp
	emcc $(INC) -c $< -o $@ -pthread

emc/obj/%.o : Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	emcc $(INC) -c $< -o $@	-pthread

emc/obj/%.o : src/%.cpp
	emcc $(INC) -c $< -o $@	-pthread

emc/obj/%.o : src/%.c
	emcc $(INC) -c $< -o $@ -pthread


all : main.exe

main.exe : $(C_OBJS) $(C++_OBJS)
	g++ -o main.exe $(C_OBJS) $(C++_OBJS) $(C_FLAGS) $(LIB)

obj/%.o : src/%.cpp
	g++ $(INC) -c $< -o $@

# obj/%.o : src/Windows/%.cpp
# 	g++ $(INC) -c $< -o $@

obj/%.o : Gb_Snd_Emu-0.1.4/%.cpp
	g++ $(INC) -c $< -o $@

obj/%.o : Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	g++ $(INC) -c $< -o $@	

obj/%.o : src/%.cpp
	g++ $(INC) -c $< -o $@	

obj/%.o : src/%.c
	gcc $(INC) -c $< -o $@

clean :
	rm main.exe obj/*.o