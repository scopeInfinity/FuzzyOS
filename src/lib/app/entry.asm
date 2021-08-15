; All user application entry point
[BITS 32]

extern main
extern exit

[SECTION .text]
_start:
        call main  ; return value should be in eax
        push eax
        call exit  ; process should be unallocated before returning from exit
