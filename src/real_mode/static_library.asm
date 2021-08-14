; Code must be within 512 bytes.
[ORG 0x7E00]
[BITS 16]

[SECTION .text]
        ; convert 4+4 byte CS:IP address into
        ; 2+2 byte CS:IP address
        pop eax
        pop ebx
        push bx
        push ax
        jmp execute_0x13_enter_real_mode

    execute_0x13_enter_real_mode:
        ; interrupts should already be disabled
        ; meant to be executed serially.
        ; mov ss, 0 ; Hack: Prerequisite

        ; enter real mode
        mov ebp, esp
        push ss ; save old absolute stack
        mov eax, cr0
        and eax, 0xFFFFFFFE
        mov cr0, eax
        jmp 0x0:execute_0x13_from_real_mode

    execute_0x13_from_real_mode:
        ; fix stack
        mov eax, 0
        mov ss, ax
        mov ds, ax  ; for loading IDT table
        lidt [idt_table_real_mode]

        ; minimal setup for real mode as required.
        mov ds, [ebp+32]
        mov es, [ebp+28]
        mov fs, [ebp+24]
        mov gs, [ebp+20]

        mov eax, [ebp+16]
        mov ebx, [ebp+12]
        mov ecx, [ebp+8]
        mov edx, [ebp+4]   ; 4 byte for far call

        int 0x13

        push eax
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax
        pop eax

        pop ss; restore absolute stack

        jmp 0x18:execute_0x13_leave_real_mode  ; Absolute Code Segment Selector

    execute_0x13_leave_real_mode:
        retf

[SECTION .data]
idt_table_real_mode     dw  0x3ff
                        db  0