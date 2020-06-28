%include "io_interface_protected.asm"
%include "keyboard.asm"

[BITS 16]

extern entry_core
global __low_panic_halt
global __low_va_args

[SECTION .text]
        jmp entry_core

        PLUGIN_SYSCALLS_IO_PROTECTED
        PLUGIN_KERNEL_DRIVERS_KEYBOARD

    __low_panic_halt:
        HLT
        JMP __low_panic_halt

    __low_va_args:

        mov esi, [esp + 0x4]     ; (index)
        shl esi, 2
        add esi, 4
        mov eax, [ebp + esi]
        ret
