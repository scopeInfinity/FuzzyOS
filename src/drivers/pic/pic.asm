[BITS 32]

global _pic_init_low
global _pic_pit_init
global _pic_writemask
global _pic_readmask

global pic_pit_set_counter
global pic_pit_get_counter
global pic_pit_reset

[SECTION .text]

    _pic_init_low:
        push ebp
        mov ebp, esp

        mov al, 0x11
        out 0x20, al
        out 0xA0, al

        ; Mapping same as BIOS default (for now)
        mov al, 0x08
        out 0x21, al
        mov al, 0x10
        out 0xA1, al

        mov al, 4
        out 0x21, al
        mov al, 2
        out 0xA1, al

        mov al, 0x01
        out 0x21, al
        out 0xA1, al

        pop ebp
        ret

    _pic_pit_init:
        push ebp
        mov ebp, esp
        ; Channel 0
        ; Low byte only
        ; Interrupt on terminal count

        ; config
        mov al, 0b00110000
        out 0x43, al

        pop ebp
        ret

    pic_pit_reset:
        mov al, 0x20
        out 0x20, al
        out 0xA0, al

        call pic_pit_reload_counter

        ret

    pic_pit_set_counter:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8] ; counter
        mov [pic_pit_counter], ax

        call pic_pit_reload_counter

        pop ebp
        ret

    pic_pit_get_counter:
        xor eax, eax
        mov ax, [pic_pit_counter]
        ret

    pic_pit_reload_counter:
        mov ax, [pic_pit_counter]
        out 0x40, al
        mov al, ah
        out 0x40, al
        ret

    _pic_readmask:
        push ebp
        mov ebp, esp

        in al, 0xA1
        mov ah, al
        in al, 0x21

        pop ebp
        ret

    _pic_writemask:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8]

        out 0x21, al
        mov al, ah
        out 0xA1, al

        pop ebp
        ret




[SECTION .data]
    pic_pit_counter             dw  0