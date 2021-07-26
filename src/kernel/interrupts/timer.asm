[BITS 32]

global irq0_pit_handler_low
extern irq0_pit_handler
extern pic_pit_reset

%macro _int_irq0_start 0
        CLI  ; should get restored on iret
        pushad ; 8*4 bytes
        mov ebp, esp

        mov ecx, [ebp+36] ; cs
        mov edi, [ebp+32] ; ip
        push ebp

        ; segment register
        push ds
        push es
        push fs
        push gs
        ; ignore ss; will handle later

        mov edx, ss
        mov esi, esp

        ; new stack and segment area
        mov esp, 0xFFFC
        mov eax, 0x10
        mov ss, eax
        mov ds, eax
        mov es, eax
        mov fs, eax
        mov gs, eax

        push edx  ; arg3: previous ss
        push esi  ; arg2: previous esp
        push ecx  ; arg1: previous cs
        push edi  ; arg0: previous ip
%endmacro

%macro _int_irq0_end 0
        ; meant to placed at end of irq handler
        pop edi  ; new ip
        pop ecx  ; new cs
        pop esi  ; new esp
        pop edx  ; new ss

        mov ss, edx
        mov esp, esi

        pop gs
        pop fs
        pop es
        pop ds

        pop ebp
        mov [ebp+36], ecx ; cs
        mov [ebp+32], edi ; ip

        popad
%endmacro

[SECTION .text]

    irq0_pit_handler_low:
        _int_irq0_start
        call irq0_pit_handler
        call pic_pit_reset
        _int_irq0_end
        iret
