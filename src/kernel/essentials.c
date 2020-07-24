#include <lib/utils/output.h>

extern unsigned int __low_va_args(unsigned int index);
#define va_args(type, index) ((type)(__low_va_args((index))))