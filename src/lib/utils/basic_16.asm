[BITS 16]

global va_args_first_adress

[SECTION .text]

    va_args_first_adress:

        mov eax, ebp
        add eax, 8
        ret
