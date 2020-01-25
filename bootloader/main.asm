; Fuzzy Bootloader
%include "constants.asm"
%include "io.asm"

[ORG 0x7C00]
[BITS 16]


[SECTION .text]

        cli
        mov ax, 0x0000
        mov es, ax                  ; es := 0          

        set_blinking 0
        print_string_ext bl_welcome, bl_welcome_len, 02, 02, C_BLACK, C_WHITE, 0
        print_string_ext bl_op_os, bl_op_os_len, 05, 04, C_WHITE, C_BLACK, 0
        print_string     bl_op_reboot, bl_op_reboot_len, 05, 05
        print_string     bl_op_shutdown, bl_op_shutdown_len, 05, 06
        print_string     bl_op_instrut, bl_op_instrut_len, 03, 08
loop:
        read_char
        put_chars_ext    al, 1, C_GREEN, C_YELLOW, 0
        jmp loop
        hlt

[SECTION .data]
    bl_welcome             db      "Fuzzy OS... (^_^)"
    bl_welcome_len         equ       ($-bl_welcome)
    bl_op_os               db      "A. Load OS"
    bl_op_os_len           equ       ($-bl_op_os)
    bl_op_reboot           db      "B. Reboot"
    bl_op_reboot_len       equ       ($-bl_op_reboot)
    bl_op_shutdown         db      "C. Shutdown"
    bl_op_shutdown_len     equ       ($-bl_op_shutdown)
    bl_op_instrut          db      "Press A/B/C key to continue..."
    bl_op_instrut_len      equ       ($-bl_op_instrut)