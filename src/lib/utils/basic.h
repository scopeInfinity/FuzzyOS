#pragma once

#define CLOCKS_PER_MS 56230 // 5% of Host CPU
#define INT_MAX 0xFFFFFFFF

// Returns address of the first variable argument of caller function.
int *va_args_first_adress();
// Returns i-th variable argument given the address of first VA.
#define get_va_arg(base, type, index) (*((type *)(((int *)base) + index)))