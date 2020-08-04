#include <lib/utils/basic.h>
#include <lib/utils/output.h>

// User should override LOG_PREFIX is required.
#define LOG_PREFIX ""

void print_log(const char *strfmt, ...) {
    int *va_base = va_args_first_address();
    int second_arg = *(int*)((&strfmt)+4);
    printf_low(strfmt, va_base+1);
    print_char('\n');
}