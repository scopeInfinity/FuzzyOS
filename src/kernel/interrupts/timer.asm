[BITS 32]

global irq0_pit_handler_low
extern irq0_pit_handler
extern pic_pit_reset

global create_infant_process_irq0_stack

%macro _int_irq0_start 0
        ; eflag, cs, ip should be at start: 0 bytes
        CLI  ; should get restored on iret
        pushad ; ADD: 8*4 bytes; start: 12 bytes
        mov ebp, esp

        mov ecx, [ebp+36] ; cs
        mov edi, [ebp+32] ; ip
        push ebp  ; ADD: 4 bytes; start: 44 bytes

        ; segment register
        push ds   ; ADD: 4 bytes; start: 48 bytes
        push es   ; ADD: 4 bytes; start: 52 bytes
        push fs   ; ADD: 4 bytes; start: 56 bytes
        push gs   ; ADD: 4 bytes; start: 60 bytes
        ; ignore ss; will handle later

        mov edx, ss
        mov esi, esp

        ; new stack and segment area
        mov esp, 0xFFF0  ; keep offset in sync with process_create(...) and create_infant_process_irq0_stack
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

    create_infant_process_irq0_stack:
        push ebp
        mov ebp, esp

        ; callee save register
        push ebx
        push esi
        push edi

        push ds
        mov ecx, [ebp+0x08]     ; arg0

        mov ds, ecx
        ; jmp aaa
        ; aaa:
        ; HLT


        ; user stack creation start
        ; HLT
        NOP
        ; user initial stack
        mov eax, 0xFFF0 ; keep in sync with _int_irq0_start
        ; kernel offset
        xor ecx, ecx
        mov [eax-0], ecx    ; user: eflag
        ; do_not_care: next two cs, ip
        ; CS and IP doesn't matter, as they are controlled by _int_irq0_end

        ; do_not_care: next four are eax, ebx, ecx, edx
        mov ecx, eax
        sub ecx, 8
        mov [eax-12-24], ecx ; pushad esp
        ; do_not_care: next three are ebp, esi, edi

        ; ebp = esp after pushd in _int_irq0_start
        mov ecx, eax
        sub ecx, 44
        mov [eax-44], ecx       ; user: ebp

        mov ecx, [ebp+0x08]     ; arg0
        mov [eax-48], ecx       ; user: ds
        mov [eax-52], ecx       ; user: es
        mov [eax-56], ecx       ; user: fs
        mov [eax-60], ecx       ; user: gs

        ; user: esp = eax-60 = 0xFFF0-60
        ; should be compatible with process_create
        ; user stack creation end

        pop ds

        pop edi
        pop esi
        pop ebx

        pop ebp
        ret
