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

%macro  put_chars 2
        ; Args: (char, count)
        mov ah, 0x09                 ; (write)
        mov al, %1                   ; (char)
        mov cx, %2                   ; (count)
        int 0x10
%endmacro

%macro  put_chars_ext 5
        ; Args: (char, count, fg_color, bg_color, page)
        mov bx, 0x%5%4%3               ; (page, attribute)
        put_chars %1, %2
%endmacro

%macro  read_char 0
        ; Output: (ah: scan code, al: ascii code)
        mov ah, 0x00                ; (read character)
        int 0x16
%endmacro

%macro  set_blinking 1
        ; Args: (should_blink)
        ; check es
        mov ax, 0x1003
        mov bx, 0x000%1
        int 10h
%endmacro

[SECTION .data]
    digit_to_hex           db      "0123456789ABCDEF"
