; Fuzzy Bootloader

ORG 0x7C00
BITS 16


SECTION .text

        mov ax, 0x0000
        mov es, ax                  ; es := 0          
        
    start: 
        mov ax, 0x1301              ; (print string, update cursor)
        mov bx, 0x0007              ; (pagenumber, attribute)
        mov bp, bl_welcome          ; (string pointer)
        mov cx, bl_welcome_sz       ; (string length)
        mov dx, bl_welcome_loc      ; (pos_y, pos_x)
        int 0x10
        hlt

SECTION .data

    bl_welcome         db      "Starting Fuzzy Bootloader... (^_^)"
    bl_welcome_sz      equ     $-bl_welcome
    bl_welcome_loc     equ     0x0101               ; (pos_y, pos_x)

