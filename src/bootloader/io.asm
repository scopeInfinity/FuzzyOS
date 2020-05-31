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

%macro  move_xy 3
        ; Args: (x, y, page)
        mov ah, 0x02                 ; (set custor position)
        mov bh, 0x%3                 ; (page number)
        mov dx, 0x%2%1               ; (pos_y, pos_x)
        int 0x10
%endmacro

%macro  print_hex_string_ext 4
        ; Args: (str, len, fg_color, page)
        mov si, %1                   ; (str)
        mov dx, %2                   ; (len)
        mov bx, 0x%4%3               ; (page, text color)
        mov ah, 0x0e                 ; (write tty)
        mov cx, 1                    ; (count)
label_marker:
        push bx
        mov bl, [si]
        and bx, 0x00F0
        shr bx, 4
        add bx, digit_to_hex
        mov al, [bx]
        pop bx
        int 0x10

        push bx
        mov bl, [si]
        and bx, 0x000F
        add bx, digit_to_hex
        mov al, [bx]
        pop bx
        int 0x10

        inc si
        sub dx, 1
        jnc label_marker
%endmacro

%macro  set_blinking 1
        ; Args: (should_blink)
        ; check es
        mov ax, 0x1003
        mov bx, 0x000%1
        int 0x10
%endmacro

%macro  clear_screen 2
        ; Output: (fg_color, bg_color)
        mov ax, 0x0600                ; (scroll 0 lines)
        mov bh, 0x%2%1                ; (attribute)
        mov cx, 0x0000                ; (window top-left RC)
        mov dx, 0x184F                ; (window bottom-right RC)
        int 0x10
%endmacro

[SECTION .data]
    digit_to_hex           db      "0123456789ABCDEF"
