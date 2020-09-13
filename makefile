INC=-IC:/SDL2/i686-w64-mingw32/include -IC:/Users/adras/Downloads/Gb_Snd_Emu-0.1.4/gb_apu
LIB=-LC:/SDL2/i686-w64-mingw32/lib -LC:/MinGW/lib -LC:/Users/adras/Downloads/Gb_Snd_Emu-0.1.4/gb_apu
C_OBJS=obj/cpu.o obj/interrupts.o obj/joypad.o \
	obj/lcd_controller.o obj/memory.o obj/opcodes.o obj/pre_opcodes.o obj/registers.o \
	obj/timers.o
C++_OBJS=obj/main.o obj/sound.o obj/Basic_Gb_Apu.o obj/Sound_Queue.o
SDL2_FLAGS=-lSDL2main -lSDL2 -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 \
    -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc
C_FLAGS=-Wall -static -std=c++11 -lmingw32 -lgbapu

all : main.exe

main.exe : $(C_OBJS) $(C++_OBJS)
	g++ -o main.exe $(C_OBJS) $(C++_OBJS) $(C_FLAGS) $(SDL2_FLAGS) $(LIB)

obj/%.o : src/%.cpp
	g++ $(INC) -c $< -o $@

# obj/%.o : src/Windows/%.cpp
# 	g++ $(INC) -c $< -o $@

obj/%.o : src/sound/%.cpp
	g++ $(INC) -c $< -o $@	

obj/%.o : src/%.cpp
	g++ $(INC) -c $< -o $@	

obj/%.o : src/%.c
	gcc $(INC) -c $< -o $@

clean :
	rm main.exe obj/*.o