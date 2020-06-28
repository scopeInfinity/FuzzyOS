#pragma once
#include <lib/utils/io.h>

#define PANIC(err, message) panic((err), (message), __FILE__, __LINE__, __SOURCE_SNAPSHOT__)

void panic(int err, const char *message, const char *src_file,
    unsigned int line_number, const char *src_snapshot);