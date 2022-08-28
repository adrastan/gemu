#include "bit_helper.h"

bool BitHelper::is_set(unsigned char byte, int bit)
{
    return (byte >> bit) & 1;
}