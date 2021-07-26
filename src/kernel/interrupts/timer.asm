[BITS 32]

global irq0_pit_handler_low
extern irq0_pit_handler
extern pic_pit_reset

%macro _int_irq0_start 0
        CLI  ; should get restored on iret
        pushad
        mov ebp, esp

        ; segment register
        push ds
        push es
        push fs
        push gs
        ; ignore ss; will handle later

        mov edx, ss
        mov ebx, ds
        mov ecx, cs
        mov esi, esp

        ; new stack and segment area
        mov esp, 0xFFFC
        mov eax, 0x10
        mov ss, eax
        mov ds, eax
        mov es, eax
        mov fs, eax
        mov gs, eax

        push edx ; previous ss
        push esi ; previous esp

        push ebx  ; arg1: previous ds
        push ecx  ; arg0: previous cs
%endmacro

%macro _int_irq0_end 0
        ; meant to placed at end of irq handler
        add esp, 8

        pop esi ; previous esp
        pop edx ; previous ss

        mov ss, edx
        mov esp, esi

        pop gs
        pop fs
        pop es
        pop ds

        popad
%endmacro

[SECTION .text]

    irq0_pit_handler_low:
        _int_irq0_start
        call irq0_pit_handler
        call pic_pit_reset
        _int_irq0_end
        iret
