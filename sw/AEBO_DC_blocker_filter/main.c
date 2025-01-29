#include <stdio.h>
#include <stdint.h>

#include "csr.h"

#define mstatus 0x300
#define MSTATUS_MAE_OFFSET  25
#define enable_AEBO set_csr(mstatus, 1<<MSTATUS_MAE_OFFSET)
#define disable_AEBO clear_csr(mstatus, 1<<MSTATUS_MAE_OFFSET)

#define EP 12            // no. elements = 2^EP
#define LIST_SIZE 1<<EP
#define SEED 0x1234

#if defined BE || defined AEBO
    #define BE_DATA 1
#else
    #define BE_DATA 0
#endif

int32_t offset_filter(int16_t [], int);
int32_t offset_filter_be(int16_t [], int);
uint16_t prand16 (uint16_t val);
uint32_t read_cycle();
uint32_t read_time();
uint32_t read_instret();

// Swap bytes in a 16-bit word
//
uint16_t swap16(uint16_t val)
{
    return (val << 8) | (val >> 8);
}

// Dump a list of 16-bit integers in native or BE format
//
// list: list of numbers
// size: list size
// be: word format is BE if not zero.
//
int dump_int16_list(int16_t list[], unsigned int size, int be)
{
    int i;
    uint16_t val, number;
    for (i = 0; i < size; i++) {
        val = list[i];
        number = be ? swap16(val) : val;
        printf("%8d (%4x)", (int16_t)number, val);
        if ((i+1) % 4 == 0) printf("\n");
    }
    if ((i) % 4 != 0) printf("\n");
}

int main()
{
    int16_t list[LIST_SIZE];
    uint16_t val = SEED;
    int32_t offset;

    // Fill the list with random values
    for (int i=0; i < LIST_SIZE; i++) {
        val = prand16(val);
#if BE_DATA == 1
        list[i] = swap16(val);
#else
        list[i] = val;
#endif
    }

    // Print initial list
    printf("Initial list:\n");
    dump_int16_list(list, LIST_SIZE, BE_DATA);

    // Read CPU timing
    uint32_t cycles = read_cycle();
    uint32_t time = read_time();
    uint32_t instret = read_instret();

    // Filter the list
#ifdef BE
    offset = offset_filter_be(list, EP);
#elif defined AEBO
    enable_AEBO;
    offset = offset_filter((void *)list+1, EP);
    disable_AEBO;
#else
    offset = offset_filter(list, EP);
#endif

    // Count elapsed CPU timing
    cycles = read_cycle() - cycles;
    time = read_time() - time;
    instret = read_instret() - instret;

    // Print final list
    printf("Final list:\n");
    dump_int16_list(list, LIST_SIZE, BE_DATA);

    printf("\n             Offset: %10d\n", offset);
    printf("  Elapsed CPU cycles: %10d\n", cycles);
    printf("   Elapsed CPU time: %10d\n", time);
    printf("Elapsed CPU instret: %10d\n\n", instret);

    return offset;
}
