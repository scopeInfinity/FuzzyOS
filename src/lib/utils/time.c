#include <lib/utils/time.h>

extern void _low_sleep(unsigned int half_instructions_count);

void sleep(unsigned int ms) {
    while (ms > 0) {
        unsigned int fms = ms > SLEEP_BURST_MS ? SLEEP_BURST_MS : ms;
        unsigned int cycles =
            CLOCKS_PER_MS * fms / SLEEP_FACTOR_DENO * SLEEP_FACTOR_NUM;
        _low_sleep(cycles);
        ms -= fms;
    }
}