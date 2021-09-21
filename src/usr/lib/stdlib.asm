[BITS 32]

global get_current_esp

[SECTION .text]

    get_current_esp:
        mov eax, esp
        ret
