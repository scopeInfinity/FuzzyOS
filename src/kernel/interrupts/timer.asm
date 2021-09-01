%include "fuzzy/memmgr/layout.asm"

[BITS 32]

global irq0_pit_handler_low
extern irq0_pit_handler
extern pit_reset

global create_infant_process_irq0_stack

%macro _int_irq0_start 0
        ; eflag, cs, ip should be at start: 0 bytes
        CLI  ; should get restored on iret
        pushad ; ADD: 8*4 bytes; start: 12 bytes

        mov ecx, [esp+36] ; cs
        mov edi, [esp+32] ; ip

        ; segment register
        push ds   ; ADD: 4 bytes; start: 44 bytes
        push es   ; ADD: 4 bytes; start: 48 bytes
        push fs   ; ADD: 4 bytes; start: 52 bytes
        push gs   ; ADD: 4 bytes; start: 56 bytes
        ; ignore ss; will handle later

        mov edx, ss
        mov esi, esp

        ; new stack and segment area
        mov esp, STACKINIT_KERNEL_EVENT
        mov eax, 0x10
        mov ss, eax
        mov ds, eax
        mov es, eax
        mov fs, eax
        mov gs, eax

        push edx  ; previous ss
        push esi  ; previous esp
        push ecx  ; previous cs
        push edi  ; previous ip

        ; C code will be using DS to read stack
        mov eax, esp
        add eax, 12
        push eax  ; arg3: &previous_ss
        sub eax, 4
        push eax  ; arg2: &previous_esp
        sub eax, 4
        push eax  ; arg1: &previous_cs
        sub eax, 4
        push eax  ; arg0: &previous_ip
%endmacro

%macro _int_irq0_end 0
        add esp, 16
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

        mov [esp+36], ecx ; cs
        mov [esp+32], edi ; ip

        popad
%endmacro

[SECTION .text]

    irq0_pit_handler_low:
        _int_irq0_start
        call irq0_pit_handler
        call pit_reset
        _int_irq0_end
        iret

    create_infant_process_irq0_stack:
        ; keep in sync with _int_irq0_start
        ; return the esp for user stack
        push ebp
        mov ebp, esp

        ; callee save register
        push ebx
        push esi
        push edi

        push ds
        mov ecx, [ebp+0x08]     ; arg0: ss
        mov eax, [ebp+0x0C]     ; arg1; user stack esp

        mov ds, ecx
        sub eax, 4

        ; kernel offset
        xor ecx, ecx

        xor ecx, 1<<9       ; enable interrupt
        mov [eax-0], ecx    ; user: eflag

        ; do_not_care: next two cs, ip
        ; CS and IP doesn't matter, as they are controlled by _int_irq0_end

        ; do_not_care: next four are eax, ebx, ecx, edx
        mov ecx, eax
        sub ecx, 8
        mov [eax-12-24], ecx ; pushad esp
        ; do_not_care: next three are ebp, esi, edi

        mov ecx, [ebp+0x08]     ; arg0
        mov [eax-44], ecx       ; user: ds
        mov [eax-48], ecx       ; user: es
        mov [eax-52], ecx       ; user: fs
        mov [eax-56], ecx       ; user: gs

        sub eax, 56 ; user stack pointer

        pop ds

        pop edi
        pop esi
        pop ebx

        pop ebp
        ret
