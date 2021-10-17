#pragma once

#include <lib/utils/basic.h>
#define SLEEP_FACTOR_NUM 3 // Possiblity of improvement.
#define SLEEP_FACTOR_DENO 2
#define SLEEP_BURST_MS                                                         \
    INT_MAX / CLOCKS_PER_MS / SLEEP_FACTOR_NUM *SLEEP_FACTOR_DENO

void sleep(unsigned int ms);