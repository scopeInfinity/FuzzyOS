#pragma once

#define LOG_VVV 0

#define MAKE_STRING(x) #x
#define TO_STRING(x) MAKE_STRING(x)

#define LOG_PREFIX __FILE__ "," TO_STRING(__LINE__) ": "

void print_log(const char *strfmt, ...);
