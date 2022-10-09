#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 0x10000

#include "system_types.h"

class Memory
{
    public:
    Memory();

    u8 memory[MEM_SIZE];
    u8 vram[16384];
    u8 wram[32768];

    u8 read_memory(u16);
};

#endif //MEMORY_H