%include "fuzzy/memmgr/tables/gdt.asm"

[BITS 32]

extern reload_idt_table
global real_mode_client

[SECTION .text]
   real_mode_client:
      push ebp
      mov ebp, esp
      ; callee save register
      push ebx
      push esi
      push edi

      ; Assumption: SS == DS
      ; DS will be changed.
      ; DO_NOT_USE stack here.
      pushf
      push ds
      push es
      push fs
      push gs
      CLI
      ; reset all segment register.

      mov eax, ss
      mov ds, eax  ; use old stack using ds
      ; create stack for real mode here.
      mov ebx, ss
      mov eax, GDT_ABS16_DS ; Absolute Data Segment Selector
      mov ss, eax
      mov eax, esp
      mov esp, 0x7DFC
      push ebx    ; push old ss to new stack
      push eax    ; push old esp in new stack


      ; push all value to be poped in real mode
      mov ecx, 0
      mov esi, ebp
      mov ebx, [ds:esi + 28]        ; (arg5:es)

      push ecx ; realmode: ds
      push ebx ; realmode: es
      push ecx ; realmode: fs
      push ecx ; realmode: gs

      mov eax, [ds:esi + 12]       ; (arg1:eax)
      mov ebx, [ds:esi + 16]       ; (arg2:ebx)
      mov ecx, [ds:esi + 20]       ; (arg3:ecx)
      mov edx, [ds:esi + 24]       ; (arg4:edx)

      push eax ; realmode: ax
      push ebx ; realmode: bx
      push ecx ; realmode: cx
      push edx ; realmode: dx

      mov eax, [ds:esi + 8]       ; (arg0:interrupt)
      push eax

      ; Note: Make sure these line have IP representable
      ; in 2 bytes.
      call GDT_ABS16_CS:0x7E00  ; execute_0x13_enter_real_mode
      add esp, 36

      ; revert back to original stack
      pop ebx
      pop ecx
      mov esp, ebx
      mov ss, ecx

      pop gs
      pop fs
      pop es
      pop ds
      push eax
      call reload_idt_table
      pop eax
      popf

      pop edi
      pop esi
      pop ebx

      pop ebp
      ret