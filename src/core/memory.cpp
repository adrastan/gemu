#include "memory.h"
#include "bit_helper.h"

Memory::Memory()
{
    this->cart = NULL;
    this->bank = 1;
    this->vram_bank = 0;
    this->wram_bank = 1;
    this->ram_bank = 0;
    this->ram_enabled = 0;

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
    if (address >= 0 && address <= 0x3FFF)
    {
        return memory[address];
    }

    if (address >= 0x4000 && address <= 0x7FFF)
    {
        return this->cart->rom[(address - 0x4000) + (this->bank * 0x4000)];
    }

    // VRAM
    if (address >= 0x8000 && address <= 0x9FFF)
    {
        return vram[(address - 0x8000) + vram_bank * 0x2000];
    }

    // External ram
    if (address >= 0xA000 && address <= 0xBFFF)
    {
        if (!ram_enabled)
        {
            return 0xFF;
        }

        if (this->cart->mbc == 2)
        {
            address &= 0x01FF;
        }

        return this->cart->ram[(address - 0xA000) + (ram_bank * 0x2000)];
    }

    // WRAM bank 0
    if (address >= 0xC000 && address <= 0xCFFF)
    {
        return wram[address - 0xC000];
    }

    // WRAM bank 1-7
    if (address >= 0xD000 && address <= 0xDFFF)
    {
        return wram[(address - 0xD000) + wram_bank * 0x1000];
    }

    if (address == 0xff00)
    {
        return joypad_state();
    }

    if (address == 0xff55)
    {
        if (dma.active)
        {
            return (u8)((dma.length / 0x10) - 1);
        }

        return memory[0xff55];
    }

    return memory[address];
}

void Memory::write_memory(u16 address, u8 byte)
{
    if (address >= 0 && address <= 0x7FFF)
    {
        if (this->cart->mbc == 1)
        {
            mbc1(address, byte);
        }
        else if (this->cart->mbc == 2)
        {
            mbc2(address, byte);
        }
        else if (this->cart->mbc == 3)
        {
            mbc3(address, byte);
        }
        else if (this->cart->mbc == 5)
        {
            mbc5(address, byte);
        }

        return;
    }

    if (address >= 0x8000 && address <= 0x9FFF)
    {
        vram[(address - 0x8000) + vram_bank * 0x2000] = byte;
        memory[address] = byte;
        return;
    }

    // External ram
    if (address >= 0xA000 && address <= 0xBFFF)
    {
        if (!ram_enabled)
        {
            return;
        }

        if (this->cart->mbc == 2)
        {
            address &= 0x01FF;
        }

        this->cart->ram[(address - 0xA000) + (ram_bank * 0x2000)] = byte;
        return;
    }

    //echo
    if (address >= 0xC000 && address <= 0xDDFF)
    {
        memory[address + 0x2000] = byte;
    }

    // WRAM bank 0
    if (address >= 0xC000 && address <= 0xCFFF)
    {
        wram[address - 0xC000] = byte;
        return;
    }

    // WRAM bank 1-7
    if (address >= 0xD000 && address <= 0xDFFF)
    {
        wram[(address - 0xD000) + wram_bank * 0x1000] = byte;
        return;
    }

    if (address >= 0xFF10 && address <= 0xFF3F)
    {
//        do_sound(address, byte);
        return;
    }
    // DMA transfer
    if (address == 0xff46)
    {
        do_oam_transfer(byte);
        memory[address] = byte;
        return;
    }
    // joypad
    if (address == 0xff00)
    {
        memory[address] &= 0xCF;
        memory[address] |= (byte & 0x30);
        return;
    }
    // enable lcd
    if (address == 0xff40)
    {
        if (BitHelper::is_set(byte,7) && !BitHelper::is_set(memory[address],7))
        {
            switch_mode(2);
            fps_count = 0;
        }

        memory[address] = byte;
        return;
    }
    // reset timers
    if (address == 0xff04)
    {
        memory[address] = 0;
        memory[address+1] = 0;
        return;
    }
    // echo memory
    if (address >= 0xE000 && address < 0xFE00)
    {
        memory[address] = byte;
        memory[address-0x2000] = byte;
        return;
    }

    if (address == 0xff51)
    {
        dma.source = ((byte << 8) | (memory[0xff52])) & 0xFFF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff52)
    {
        dma.source = ((memory[0xff51] << 8) | (byte)) & 0xFFF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff53)
    {
        dma.dest = ((byte << 8) | (memory[0xff54])) & 0x1FF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff54)
    {
        dma.dest = ((memory[0xff53] << 8) | (byte)) & 0x1FF0;
        memory[address] = byte;
        return;
    }

    if (address == 0xff55)
    {
        do_dma(byte);
        return;
    }

    if (address == 0xff68)
    {
        bg_palette.auto_inc = is_set(byte, 7);
        bg_palette.index = 0x3f & byte;
        memory[address] = byte;
        return;
    }

    if (address == 0xff69)
    {
        bg_palette.palette[bg_palette.index] = byte;
        if (bg_palette.auto_inc)
        {
            ++bg_palette.index;
            if (bg_palette.index > 0x3f)
            {
                bg_palette.index = 0;
            }
        }

        memory[address] = byte;
        return;
    }

    if (address == 0xff6A)
    {
        spr_palette.auto_inc = is_set(byte, 7);
        spr_palette.index = 0x3f & byte;
        memory[address] = byte;
        return;
    }

    if (address == 0xff6B)
    {
        spr_palette.palette[spr_palette.index] = byte;
        if (spr_palette.auto_inc)
        {
            spr_palette.index++;
            if (spr_palette.index > 0x3f)
            {
                spr_palette.index = 0;
            }
        }
        memory[address] = byte;
        return;
    }

    // unusable memory
    if (address >= 0xFEA0 && address <= 0xFEFF)
    {
        return;
    }

    if (address == 0xff4f && headers.cgb)
    {
        vram_bank = 1 & byte;
    }

    if (address == 0xff70 && headers.cgb)
    {
        wram_bank = byte & 7;
        if (!wram_bank)
        {
            wram_bank = 1;
        }
    }

    if (headers.cgb && address == 0xFF4D)
    {
        if (is_set(byte, 0))
        {
            printf("prepare speed %d\n", double_speed);
            prepare_speed = 1;
        }
        memory[address] = byte;
        return;
    }

    if (headers.cgb && address == 0xFF56)
    {
        printf("infrared not supported\n");
    }

    memory[address] = byte;
}

void Memory::mbc1(u16 address, u8 byte)
{
    static u16 rom_bank_lower = 0;
    static u16 rom_bank_upper = 0;

    if (address >= 0 && address <= 0x1FFF)
    {
        ram_enabled = (byte & 0x0A) == 0x0A;
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        if (!byte && bank_mode == 0)
        {
            byte = 1;
        }
        if (this->cart->rom_size == 1)
        {
            rom_bank_lower = byte & 0x7;
        }
        else if (this->cart->rom_size == 2)
        {
            rom_bank_lower = byte & 0xF;
        }
        else if (this->cart->rom_size == 3)
        {
            rom_bank_lower = byte & 0xF;
        }
        else
        {
            rom_bank_lower = byte & 0x1F;
        }
        bank = rom_bank_lower;
    }
    else if (address >= 0x4000 && address <= 0x5FFF)
    {
        if (bank_mode == 0 && this->cart->rom_size >= 5)
        {
            rom_bank_upper = byte & 3;
            bank = (rom_bank_upper << 5) | rom_bank_lower;
        }
        else if (bank_mode == 1)
        {
            ram_bank = byte & 3;
        }
    }
    else if (address >= 0x6000 && address <= 0x7FFF)
    {
        bank_mode = byte & 1;
    }
}

void Memory::mbc2(u16 address, u8 byte)
{
    if (address <= 0x3FFF)
    {
        if (!BitHelper::is_set(address, 8))
        {
            ram_enabled =(byte & 0x0A) == 0x0A;
        }
        else
        {
            bank = byte & 0x0F;
            if (!bank)
            {
                bank = 1;
            }
        }
    }
}

void Memory::mbc3(u16 address, u8 byte)
{
    if (address >= 0 && address <= 0x1FFF)
    {
        ram_enabled =(byte & 0x0A) == 0x0A;
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        if (!byte)
        {
            byte = 1;
        }
        bank = byte & 0x7F;
    }
    else if (address >= 0x4000 && address <= 0x5FFF)
    {
        if (byte >= 0 && byte <= 0x3)
        {
            ram_bank = 3 & byte;
            rtc_select = 0;
        }
        else if (byte >= 0x08 && byte <= 0x0C)
        {
            rtc_select = 1;
            ram_bank = byte >> 3;
        }
    }
}

void Memory::mbc5(u16 address, u8 byte)
{
    if (address >= 0 && address <= 0x1FFF)
    {
        ram_enabled = (byte & 0x0A) == 0x0A;
    }
    else if (address >= 0x2000 && address <= 0x2FFF)
    {
        bank = byte;
    }
    else if (address >= 0x3000 && address <= 0x3FFF)
    {
        bank = ((byte & 0x1) << 8) | (bank & 0xFF);
    }
    else if (address >= 0x4000 && address <= 0x5FFF)
    {
        ram_bank = byte & 0xf;
    }
    if (!bank)
    {
        bank = 1;
    }
}

void Memory::do_oam_transfer(u8 byte)
{
    int b = (int)byte << 8;
    int c = 0x00;
    for (int i = b; i < b + 0xA0; ++i) {
        memory[0xfe00+c] = memory[i];
        ++c;
    }
}

void Memory::do_dma(u8 byte)
{
    if (!BitHelper::is_set(byte, 7))
    {
        if (dma.active)
        {
            dma.active = 0;
        }
        else
        {
            do_gdma(byte);
        }
    }
    else {
        dma.length = ((byte & 0x7f) + 1) * 0x10;
        dma.position = 0;
        dma.active = 1;
    }
}

void Memory::do_gdma(u8 byte)
{
    int length = ((byte & 0x7F) + 1) * 0x10;
    for (int i = 0; i < length; ++i)
    {
        write_memory(dma.dest + 0x8000 + i, read_memory(dma.source + i));
    }

    memory[0xff55] = 0xff;
}

void Memory::do_hdma()
{
    for (int i = 0; i < 0x10; ++i)
    {
        if (dma.dest + 0x8000 + i + dma.position > 0x9FFF)
        {
            dma.active = 0;
            memory[0xff55] = 0xff;
            return;
        }
        write_memory(dma.dest + 0x8000 + i + dma.position, read_memory(dma.source + i + dma.position));
        dma.length--;
    }

    dma.position += 0x10;
    if (dma.length == 0)
    {
        memory[0xff55] = 0xff;
        dma.active = 0;
    }
}

u8 Memory::joypad_state()
{
    if (!BitHelper::is_set(memory[0xff00],4))
    {
        u8 dir_buttons = 0xFF & 0xF;
        dir_buttons |= 0xF0;
        return dir_buttons & memory[0xff00];
    }
    else if (!BitHelper::is_set(memory[0xff00],5))
    {
        u8 std_buttons = 0xFF & 0xF0;
        std_buttons >>= 4;
        std_buttons |= 0xF0;
        return std_buttons & memory[0xff00];
    }

    return 0xff;
}
