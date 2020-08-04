[BITS 32]

global va_args_first_address

[SECTION .text]

    va_args_first_address:

        mov eax, ebp
        add eax, 8
        ret
