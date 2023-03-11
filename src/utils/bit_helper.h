#ifndef BIT_HELPER
#define BIT_HELPER

#include <cstdint>

class BitHelper
{
    public:
    static bool is_set(unsigned char, int);
    static bool half_carry(uint8_t, uint8_t);
};

#endif //BIT_HELPER
