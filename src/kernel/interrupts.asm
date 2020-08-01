[BITS 32]

global interrupt_nohup
global load_idt_table_low
global kernel_enable_interrupts
global kernel_disable_interrupts
global idt_table

global syscall_interrupt_handler_low
extern syscall_interrupt_handler
global syscall_interrupt_keyboard_getch_low
extern syscall_interrupt_keyboard_getch

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

    syscall_interrupt_handler_low:
        call syscall_interrupt_handler
        iret

    syscall_interrupt_keyboard_getch_low:
        push ds
        mov bx, 0x10
        mov ds, bx
        call syscall_interrupt_keyboard_getch
        pop ds
        iret

