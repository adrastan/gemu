#include "bit_helper.h"
#include <cstdint>

bool BitHelper::is_set(unsigned char byte, int bit)
{
    return (byte >> bit) & 1;
}

bool BitHelper::half_carry(uint8_t a, uint8_t b)
{
    return (((a & 0xf) + (b & 0xf)) & 0x10) == 0x10;
}

void BitHelper::set_bit(uint8_t *byte, int bit)
{
    *byte |= (1 << bit);
}

void BitHelper::reset_bit(uint8_t *byte, int bit)
{
    *byte &= ~(1 << bit);
}
