#include "memory.h"

Memory::Memory()
{
    for (int i = 0x8000; i < 0x10000; ++i)
    {
        memory[i] = 0;
    }
    for (int i = 0; i < 16384; ++i)
    {
        vram[i] = 0;
    }
    for (int i = 0; i < 32768; ++i)
    {
        wram[i] = 0;
    }
    memory[0xFF05] = 0;
    memory[0xFF06] = 0;
    memory[0xFF07] = 0;
    memory[0xFF40] = 0x91;
    memory[0xFF42] = 0;
    memory[0xFF43] = 0;
    memory[0xFF45] = 0;
    memory[0xFF47] = 0xFC;
    memory[0xFF48] = 0xFF;
    memory[0xFF49] = 0xFF;
    memory[0xFF4A] = 0;
    memory[0xFF4B] = 0;
    memory[0xFFFF] = 0;
    memory[0xFF00] = 0xFF;
    memory[0xFF02] = 0x7C;
}

u8 Memory::read_memory(u16 address)
{
    return memory[address];
}