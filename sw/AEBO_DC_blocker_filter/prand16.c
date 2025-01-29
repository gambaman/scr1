// Return pseudo-random 16 bit word using lfsr algorithm

// Taps: 15, 14, 12, 3 (https://docs.xilinx.com/v/u/en-US/xapp052)
// Input
//   val: initial value
// Return
//   return[15:0]: repeat 16 ( SHL(a0, a[15]^a[14]^a[12]^a[3]) )

#include <stdint.h>

uint16_t prand16 (uint16_t val)
{
    uint16_t bit;
    for (int i=0; i<16; i++) {
        bit = ((val >> 3) ^ (val >> 12) ^ (val >> 14) ^ (val >> 15)) & 1;
        val = (val << 1) | bit;
    }
    return val;
}
