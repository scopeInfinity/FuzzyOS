#ifndef __LIB_SYSCALLS_TIME
#define __LIB_SYSCALLS_TIME

#include "lib/syscalls/basic.h"
#define SLEEP_FACTOR_NUM         3     // Possiblity of improvement.
#define SLEEP_FACTOR_DENO        2
#define SLEEP_BURST_MS           INT_MAX/CLOCKS_PER_MS/SLEEP_FACTOR_NUM*SLEEP_FACTOR_DENO

extern void _low_sleep(unsigned int half_instructions_count);

void sleep(unsigned int ms) {
    while(ms>0) {
        unsigned int fms = ms>SLEEP_BURST_MS?SLEEP_BURST_MS:ms;
        unsigned int cycles = CLOCKS_PER_MS*fms/SLEEP_FACTOR_DENO*SLEEP_FACTOR_NUM;
        _low_sleep(cycles);
        ms-=fms;
    }
}

#endif