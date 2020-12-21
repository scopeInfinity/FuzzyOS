#include <lib/utils/basic.h>
#include <lib/utils/output.h>

void print_log(const char *strfmt, ...) {
    int *va_base = va_args_first_address(&strfmt);
    printf_low(strfmt, va_base);
    print_char('\n');
}
