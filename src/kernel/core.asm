[BITS 32]

extern kernel_core_entry
extern reload_idt_table
global kernel_core_entry_asm

[SECTION .text]
        ; protected mode real entry point.
        CLI
        mov ax, 0x10
        mov es, ax
        mov ss, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax

        ; TODO: Increase kernel code stack memory size
        ; The value also depends on kernel handler stack size
        mov esp, 0xC000  ; init stack pointer
        jmp kernel_core_entry

        ; kernel_core_entry_asm currently exists only for tests.
    kernel_core_entry_asm:
        ; __TEST_INJECT_KERNEL_CORE_ENTRY__: mov eax, 0x922E52FF
        ; __TEST_INJECT_KERNEL_CORE_ENTRY__: HLT
        ret