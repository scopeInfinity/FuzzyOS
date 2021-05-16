#include <lib/utils/basic.h>
#include <lib/utils/output.h>
#include <lib/utils/panic.h>

// User should override LOG_PREFIX is required.
#define LOG_PREFIX ""

void print_log(const char *strfmt, ...) {
    int *va_base = va_args_first_adress();
    print_line(LOG_PREFIX);
    print_line(strfmt);
    // TODO(scopeinfinity): fix variable arguments.
    // printf_low(strfmt, va_base+1);
    print_char('\n');
}