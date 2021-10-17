#pragma once

#include <lib/utils/output.h>

#define PANIC(err, message) panic((err), (message), __FILE__, __LINE__)

#define ASSERT(ok) (ok || panic(0, "Assert Failed: " #ok, __FILE__, __LINE__))

void panic_just_halt();
int panic(int err, const char *message, const char *src_file,
          unsigned int line_number);
int panic_screen_init();
