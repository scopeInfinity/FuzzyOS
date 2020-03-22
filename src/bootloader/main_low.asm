%include "constants.asm"
;;%include "io.asm"
%macro  print_string 4
        ; Args: (str, len, x, y)
        ; check es, bx
        mov ax, 0x1301              ; (print string, update cursor)
        mov bp, %1                  ; (es:bp string pointer)
        mov cx, %2                  ; (string length)
        mov dx, 0x%4%3              ; (pos_y, pos_x)
        int 0x10
%endmacro

%macro  print_string_ext 7
        ; Args: (str, len, x, y, fg_color, bg_color, page)
        ; check es
        mov bx, 0x%7%6%5            ; (pagenumber, attribute)
        print_string %1, %2, %3, %4
%endmacro

global _low_print
[SECTION .text]

    _low_print:
        push rbp
        mov rbp, rsp
        print_string_ext bl_welcome, bl_welcome_len, 02, 02, C_BLACK, C_WHITE, 0
        mov rax, 15
        leave
        ret

[SECTION .data]
    bl_welcome             db      "TESTTT"
    bl_welcome_len         equ       ($-bl_welcome)
