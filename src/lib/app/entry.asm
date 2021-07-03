; All user application entry point
[BITS 32]

extern main
extern entry_console_init

[SECTION .text]
_start:
        call entry_console_init
        call main
        retf


