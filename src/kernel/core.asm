%include "io_interface_protected.asm"

[BITS 16]

extern entry_core

[SECTION .text]
        jmp entry_core

        PLUGIN_SYSCALLS_IO_PROTECTED

