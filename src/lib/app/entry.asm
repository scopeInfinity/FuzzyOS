; Any app entry point
%include "constants.asm"
%include "io_interface_protected.asm"
%include "time_syscall.asm"
%include "disk_syscall.asm"

[BITS 16]

extern main

[SECTION .text]
        jmp main

        PLUGIN_SYSCALLS_IO_PROTECTED
        PLUGIN_SYSCALLS_TIME
        PLUGIN_SYSCALLS_DISK
