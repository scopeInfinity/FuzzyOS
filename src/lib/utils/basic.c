#include <lib/utils/basic.h>

int* va_args_first_address(void *last_positional_argument_reference) {
    return (int*)(last_positional_argument_reference+4);
}
