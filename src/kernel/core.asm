%include "stub.asm"

[BITS 32]

extern kernel_core_entry
global __low_va_args

[SECTION .text]
       ; protected mode real entry point.
        CLI
        mov ax, 0x10
        mov es, ax
        mov ss, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax

        ; transfer control to the desired entry point.
        get_protected_mode_entry
        cmp eax, 0
        je kernel_core_entry
        unshelve_protected_mode_jump_entry_address_fpm

    __low_va_args:

        mov esi, [esp + 0x4]     ; (index)
        shl esi, 2
        add esi, 4
        mov eax, [ebp + esi]
        ret
