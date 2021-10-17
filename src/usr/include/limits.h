#pragma once

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1)
#define ULLONG_MAX ((unsigned long)(((unsigned long)LLONG_MAX) * 2ULL + 1))

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX ((unsigned int)(((unsigned int)INT_MAX) * 2U + 1))
