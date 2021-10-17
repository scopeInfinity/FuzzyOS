#pragma once

#define __MACRO_TO_STRING_INTERNAL(x) #x
#define MACRO_TO_STRING(x) __MACRO_TO_STRING_INTERNAL(x)
#define __STR__LINE__ MACRO_TO_STRING(__LINE__)

#define VERIFY_STACKGUARD()                                                    \
    (verify_stack_guard(__FILE__ "[" __STR__LINE__                             \
                                 "]; verify_stack_guard failed"))

void verify_stack_guard(char err_message[]);
