[BITS 16]

global _low_sleep

[SECTION .text]
    _low_sleep:
        push ebp
        mov ebp, esp
        mov ebx, [ebp + 0x08]          ; (cycles)
        _low_sleep_internal:
        sub ebx, 1
        jnc _low_sleep_internal

        mov esp, ebp
        pop ebp
        ret