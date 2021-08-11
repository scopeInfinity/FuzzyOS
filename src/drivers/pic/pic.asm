[BITS 32]

global _pic_init_low
global _pic_pit_init
global _pic_writemask
global _pic_readmask

global pic_pit_set_counter
global pic_pit_get_counter
global pic_pit_reset

PIC1 EQU 0x20
PIC2 EQU 0xA0
PIC1_CMD EQU PIC1
PIC2_CMD EQU PIC2
PIC1_DATA EQU (PIC1+1)
PIC2_DATA EQU (PIC2+1)

[SECTION .text]

    _pic_init_low:
        push ebp
        mov ebp, esp

        mov al, 0x11
        out PIC1_CMD, al
        out PIC2_CMD, al

        ; Remap IRQ
        ; FuzzyOS won't be using IRQ0 in real mode.
        mov eax, [ebp + 0x08] ; idt_irq0_pic1
        out PIC1_DATA, al
        mov eax, [ebp + 0x0C] ; idt_irq0_pic2
        out PIC2_DATA, al

        mov al, 4
        out PIC1_DATA, al
        mov al, 2
        out PIC2_DATA, al

        mov al, 0x01
        out PIC1_DATA, al
        out PIC2_DATA, al

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
        out PIC1_CMD, al
        out PIC2_CMD, al

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

        in al, PIC2_DATA
        mov ah, al
        in al, PIC1_DATA

        pop ebp
        ret

    _pic_writemask:
        push ebp
        mov ebp, esp

        mov eax, [ebp + 0x8]

        out PIC1_DATA, al
        mov al, ah
        out PIC2_DATA, al

        pop ebp
        ret




[SECTION .data]
    pic_pit_counter             dw  0