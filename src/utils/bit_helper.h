#ifndef BIT_HELPER
#define BIT_HELPER

#include <cstdint>

class BitHelper
{
    public:
    static bool is_set(unsigned char, int);
    static bool half_carry(uint8_t, uint8_t);
    static void set_bit(uint8_t *byte, int bit);
    static void reset_bit(uint8_t *byte, int bit);
};

#endif //BIT_HELPER
