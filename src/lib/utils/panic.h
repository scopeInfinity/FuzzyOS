#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <lib/utils/io.h>

#define PANIC(err, message) panic((err), (message), __FILE__, __LINE__)

void panic_just_halt();
void panic(int err, const char *message, const char *src_file,
    unsigned int line_number);

#ifdef __cplusplus
}
#endif