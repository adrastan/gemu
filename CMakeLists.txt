cmake_minimum_required(VERSION 3.7...3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(gemu VERSION 1.0 DESCRIPTION "Game Boy Emulator")

find_package(SDL2 REQUIRED)

set(source_files
    src/core/cpu.cpp
    src/core/interrupts.cpp
    src/core/joypad.cpp
    src/core/lcd_controller.cpp
    src/core/memory.cpp
    src/core/opcodes.cpp
    src/core/pre_opcodes.cpp
    src/core/registers.cpp
    src/core/sound.cpp
    src/core/timers.cpp
)
set(header_files
    src/core/cpu_types.h
    src/core/cpu.h
    src/core/interrupts.h
    src/core/joypad.h
    src/core/lcd_controller.h
    src/core/memory.h
    src/core/opcodes.h
    src/core/pre_opcodes.h
    src/core/registers.h
    src/core/sound.h
    src/core/timers.h
)

set(sound_apu 
    src/external/GB_Snd_Emu-0.1.4/Basic_Gb_Apu.cpp
    src/external/GB_Snd_Emu-0.1.4/Basic_Gb_Apu.h
    src/external/GB_Snd_Emu-0.1.4/Sound_Queue.cpp
    src/external/GB_Snd_Emu-0.1.4/Sound_Queue.h
    src/external/GB_Snd_Emu-0.1.4/Wave_Writer.cpp
    src/external/GB_Snd_Emu-0.1.4/Wave_Writer.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/blargg_common.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/blargg_source.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Blip_Buffer.cpp
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Blip_Buffer.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Blip_Synth.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Gb_Apu.cpp
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Gb_Apu.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Gb_Oscs.cpp
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Gb_Oscs.h
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Multi_Buffer.cpp
    src/external/GB_Snd_Emu-0.1.4/gb_apu/Multi_Buffer.h
    src/external/GB_Snd_Emu-0.1.4/boost/config.hpp
    src/external/GB_Snd_Emu-0.1.4/boost/cstdint.hpp
    src/external/GB_Snd_Emu-0.1.4/boost/static_assert.hpp
)

add_library(gemu ${source_files} ${header_files})
add_library(sound_apu ${sound_apu})
include_directories(src/external/Gb_Snd_Emu-0.1.4 src/external/Gb_Snd_Emu-0.1.4/gb_apu ${SDL2_INCLUDE_DIRS})

target_link_libraries(sound_apu PRIVATE SDL2::SDL2)
target_link_libraries(gemu PRIVATE SDL2::SDL2 sound_apu -lmingw32 -mwindows)
include_directories(src/core)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE gemu)

#add_subdirectory(src/gui)
