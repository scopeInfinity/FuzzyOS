%include "fuzzy/memmgr/layout.asm"
%include "fuzzy/memmgr/tables/gdt.asm"

[BITS 32]

extern kernel_core_entry
extern reload_idt_table
global kernel_core_entry_asm

[SECTION .text]
        ; protected mode real entry point.
        CLI
        mov ax, GDT_KERNEL_DS
        mov es, ax
        mov ss, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax

        xor ebp, ebp    ; origin of stack trace
        mov esp, STACKINIT_KERNEL_CORE  ; init stack pointer
        jmp kernel_core_entry

        ; kernel_core_entry_asm currently exists only for tests.
    kernel_core_entry_asm:
        ; __TEST_INJECT_KERNEL_CORE_ENTRY__: mov eax, 0x922E52FF
        ; __TEST_INJECT_KERNEL_CORE_ENTRY__: HLT
        ret