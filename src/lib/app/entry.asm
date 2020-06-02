; Any app entry point
%include "constants.asm"
%include "io.asm"
%include "io_syscall.asm"
%include "time_syscall.asm"
%include "disk_syscall.asm"

[BITS 16]

extern main

[SECTION .text]
        jmp main

        PLUGIN_SYSCALLS_IO
        PLUGIN_SYSCALLS_TIME
        PLUGIN_SYSCALLS_DISK
