#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <memory>

#include "system_types.h"

class Cartridge
{
    public:
    Cartridge(u8 * const, int);
    ~Cartridge();

    std::unique_ptr<u8[]> rom;
    int size;
    int rom_size;
    int ram_size;
    int mbc;

    bool is_ram_enabled;
    bool is_cgb;
    bool is_sgb;

    char logo[48];
    char title[16];

    void print();
};

#endif //CARTRIDGE_H