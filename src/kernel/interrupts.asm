%include "process_macro.asm"
[BITS 32]

global interrupt_nohup
global load_idt_table_low
global kernel_enable_interrupts
global kernel_disable_interrupts
global idt_table

global irq0_interrupt_timer_handler_low
global irq0_interrupt_timer_handler_low_shelved
global irq0_interrupt_timer_handler_low_unshelved
extern irq0_interrupt_timer_handler

global syscall_selector_low
extern syscall_selector

extern pic_end_of_interrupt
extern pic_timer_reload_counter

PROCESS_MACRO_USER

[SECTION .text]

    interrupt_nohup:
        iret

    load_idt_table_low:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8]
        lidt [eax]

        mov esp, ebp
        pop ebp
        ret

    kernel_enable_interrupts:
        STI
        ret

    kernel_disable_interrupts:
        CLI
        ret

    irq0_interrupt_timer_handler_low:
        pushfd
        pushad
        PROCESS_SHELVE
        call irq0_interrupt_timer_handler
        call pic_end_of_interrupt
        call pic_timer_reload_counter
        PROCESS_UNSHELVE
        popad
        popfd
        iret


    syscall_selector_low:
        ; Assumes SS and DS to remain same.
        pushfd
        PROCESS_SHELVE
        ; TODO: CLI makes getch() to block IRQ0
        ; Added CLI because process_shelve stores
        ; nested process_shelve esp, ss in a same variable
        ; thus breaking nested calls.
        CLI

        push esi
        push edx
        push ecx
        push ebx
        push eax
        call syscall_selector
        add esp, 20

        PROCESS_UNSHELVE
        popfd
        iret
