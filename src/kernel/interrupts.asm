[BITS 32]

global interrupt_nohup
global load_idt_table
global kernel_enable_interrupts
global kernel_disable_interrupts
global idt_table

global irq0_interrupt_timer_handler_low
global enable_timer_interrupt

global syscall_interrupt_handler_low
extern syscall_interrupt_handler

[SECTION .text]

    interrupt_nohup:
        iret

    load_idt_table:
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

    irq0_interrupt_timer_handler_low:
        call syscall_interrupt_handler
        iret

    enable_timer_interrupt:
        ; Channel 0
        ; Low byte only
        ; Interrupt on terminal count
        CLI
        mov al, 0b00110000
        out 0x43, al

        ; Time: ~1200us
        mov ax, 1000
        out 0x40, al
        mov al, ah
        out 0x40, al
        STI
        ret
