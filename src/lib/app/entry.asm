; All user application entry point
[BITS 32]

extern main

[SECTION .text]
_start:
        call main
        retf


