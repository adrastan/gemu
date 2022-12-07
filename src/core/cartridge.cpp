#include <iostream>
#include "cartridge.h"
#include "bit_helper.h"
#include "logger.h"

Cartridge::Cartridge(u8 * const buf, int size)
    : rom(std::make_unique<u8[]>(size))
{
    this->size = size;

    // Load file into cart rom
    for (int i = 0; i < size; ++i)
    {
        rom[i] = buf[i];
    }

    // Init cart ram
    for (int i = 0; i < 131072; ++i) {
        ram[i] = 0;
    }

    this->is_ram_enabled = this->rom[0x149];

    // Get Memory Bank Controller
    if (this->rom[0x147] >= 1 && this->rom[0x147] <= 3)
    {
        this->mbc = 1;
    }
    else if (this->rom[0x147] == 5 || this->rom[0x147] == 6)
    {
        this->mbc = 2;
    }
    else if (this->rom[0x147] >= 0x0f && this->rom[0x147] <= 0x13)
    {
        this->mbc = 3;
    }
    else if (this->rom[0x147] >= 0x19 && this->rom[0x147] <= 0x1e)
    {
        this->mbc = 5;
    }

    // Get logo
    for (int i = 0; i < 48; ++i)
    {
        this->logo[i] = this->rom[0x104 + i];
    }

    // Get title
    for (int i = 0; i < 16; ++i)
    {
        this->title[i] = this->rom[0x134 + i];
    }

    this->is_cgb = BitHelper::is_set(this->rom[0x143], 7);
    this->is_sgb = this->rom[0x146] == 0x03;
    this->rom_size = this->rom[0x148];
    this->ram_size = this->rom[0x149];
}

Cartridge::~Cartridge()
{
    // delete[] this->rom;
}

void Cartridge::print()
{
    std::string title(this->title);
    Logger::log("\nTitle: " + title);
    Logger::log("MBC: " + std::to_string(this->mbc));
    Logger::log("Rom size: " + std::to_string(this->rom_size));
    Logger::log("Ram size: " + std::to_string(this->ram_size));
    Logger::log("\n");
}
