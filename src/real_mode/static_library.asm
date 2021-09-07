%include "fuzzy/memmgr/tables/gdt.asm"

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
        jmp execute_int_enter_real_mode

    execute_int_enter_real_mode:
        ; interrupts should already be disabled
        ; meant to be executed serially.
        ; mov ss, 0 ; Hack: Prerequisite

        ; enter real mode
        mov ebp, esp
        push ss ; save old absolute stack
        mov eax, cr0
        and eax, 0xFFFFFFFE
        mov cr0, eax
        jmp GDT_NULL_CS:execute_int_from_real_mode

    execute_int_from_real_mode:
        ; fix stack
        mov eax, 0
        mov ss, ax
        mov ds, ax  ; for loading IDT table
        lidt [idt_table_real_mode]

        ; minimal setup for real mode as required.
        mov ds, [ebp+36]
        mov es, [ebp+32]
        mov fs, [ebp+28]
        mov gs, [ebp+24]

        mov eax, [ebp+20]
        mov ebx, [ebp+16]
        mov ecx, [ebp+12]
        mov edx, [ebp+8]

        ; interrupt number
        mov esi, [ebp+4]   ; 4 byte for far call

        ; check for some interrupt numbers only
        cmp esi, 0x10
        je _int_0x10
        cmp esi, 0x13
        je _int_0x13

        jmp _int_end  ; found no match

    _int_0x10:
        int 0x10
        jmp _int_end

    _int_0x13:
        int 0x13
        jmp _int_end

    _int_end:
        push eax
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax
        pop eax

        pop ss; restore absolute stack

        jmp GDT_ABS16_CS:execute_int_leave_real_mode  ; Absolute Code Segment Selector

    execute_int_leave_real_mode:
        retf

[SECTION .data]
idt_table_real_mode     dw  0x3ff
                        db  0