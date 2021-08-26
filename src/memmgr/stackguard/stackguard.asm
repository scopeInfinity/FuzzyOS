; Corresponding object file should be linked at the tail of the
; instructions.
; Assumes .data gets linked after .text

[BITS 32]

global check_stack_guard

; it would be nice if magic number is present at head of
; instruction but this should be enough for now.
STACK_GUARD_MAGIC_NUMBER    EQU     0x4C10A5C7

[SECTION .text]
    check_stack_guard:
        ; return 0 is stack is good
        push ebp
        mov ebp, esp

        mov eax, [stack_guard_pointer]
        xor eax, STACK_GUARD_MAGIC_NUMBER

        pop ebp
        ret

[SECTION .data]
    stack_guard_pointer     dd      STACK_GUARD_MAGIC_NUMBER