#include "bit_helper.h"

bool BitHelper::is_set(unsigned char byte, int bit)
{
    return (byte >> bit) & 1;
}

bool BitHelper::half_carry(uint8_t a, uint8_t b)
{
    return (((a & 0xf) + (b & 0xf)) & 0x10) == 0x10;
}
