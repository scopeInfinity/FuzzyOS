[BITS 32]

extern interrupt_handler_0x00_0x1F_exception

global _interrupt_handler_0x00_exception
global _interrupt_handler_0x01_exception
global _interrupt_handler_0x02_exception
global _interrupt_handler_0x03_exception
global _interrupt_handler_0x04_exception
global _interrupt_handler_0x05_exception
global _interrupt_handler_0x06_exception
global _interrupt_handler_0x07_exception
global _interrupt_handler_0x08_exception
global _interrupt_handler_0x09_exception
global _interrupt_handler_0x0A_exception
global _interrupt_handler_0x0B_exception
global _interrupt_handler_0x0C_exception
global _interrupt_handler_0x0D_exception
global _interrupt_handler_0x0E_exception
global _interrupt_handler_0x0F_exception
global _interrupt_handler_0x10_exception
global _interrupt_handler_0x11_exception
global _interrupt_handler_0x12_exception
global _interrupt_handler_0x13_exception
global _interrupt_handler_0x14_exception
global _interrupt_handler_0x15_exception
global _interrupt_handler_0x16_exception
global _interrupt_handler_0x17_exception
global _interrupt_handler_0x18_exception
global _interrupt_handler_0x19_exception
global _interrupt_handler_0x1A_exception
global _interrupt_handler_0x1B_exception
global _interrupt_handler_0x1C_exception
global _interrupt_handler_0x1D_exception
global _interrupt_handler_0x1E_exception
global _interrupt_handler_0x1F_exception

[SECTION .text]

%macro create_low__interrupt_handler_xy_exception 1
    _interrupt_handler_%1_exception:
        ; As we are HLT at end of exception there
        ; is no need to save context for now.
        xor eax, eax
        mov eax, ds    ; execution ds
        push eax
        xor eax, eax
        mov eax, ss    ; execution ss (current also)
        push eax
        mov eax, 0x10  ; kernel ds
        mov ds, eax
        mov eax, %1
        push eax
        call interrupt_handler_0x00_0x1F_exception
        HLT
%endmacro

%macro create_low__interrupt_handler_xy_exception_nohup 1
    _interrupt_handler_%1_exception:
        iret
%endmacro

    create_low__interrupt_handler_xy_exception_nohup 0x00  ; divide-by-zero
    create_low__interrupt_handler_xy_exception 0x01
    create_low__interrupt_handler_xy_exception 0x02
    create_low__interrupt_handler_xy_exception 0x03
    create_low__interrupt_handler_xy_exception 0x04
    create_low__interrupt_handler_xy_exception_nohup 0x05  ; bound-range-exceeded
    create_low__interrupt_handler_xy_exception 0x06
    create_low__interrupt_handler_xy_exception 0x07
    create_low__interrupt_handler_xy_exception 0x08
    create_low__interrupt_handler_xy_exception 0x09
    create_low__interrupt_handler_xy_exception 0x0A
    create_low__interrupt_handler_xy_exception 0x0B
    create_low__interrupt_handler_xy_exception 0x0C
    create_low__interrupt_handler_xy_exception 0x0D
    create_low__interrupt_handler_xy_exception 0x0E
    create_low__interrupt_handler_xy_exception 0x0F
    create_low__interrupt_handler_xy_exception 0x10
    create_low__interrupt_handler_xy_exception 0x11
    create_low__interrupt_handler_xy_exception 0x12
    create_low__interrupt_handler_xy_exception 0x13
    create_low__interrupt_handler_xy_exception 0x14
    create_low__interrupt_handler_xy_exception 0x15
    create_low__interrupt_handler_xy_exception 0x16
    create_low__interrupt_handler_xy_exception 0x17
    create_low__interrupt_handler_xy_exception 0x18
    create_low__interrupt_handler_xy_exception 0x19
    create_low__interrupt_handler_xy_exception 0x1A
    create_low__interrupt_handler_xy_exception 0x1B
    create_low__interrupt_handler_xy_exception 0x1C
    create_low__interrupt_handler_xy_exception 0x1D
    create_low__interrupt_handler_xy_exception 0x1E
    create_low__interrupt_handler_xy_exception 0x1F
