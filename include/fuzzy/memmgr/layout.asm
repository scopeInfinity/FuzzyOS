; constants only

; START_ENSURE_SAME_layout_h
MEMORY_APP_SIZE          EQU  0x20000
MEMORY_KERNEL_SIZE       EQU  0x64000
; END_ENSURE_SAME_layout_h

; Kernel core is app with pid 0
; Kernel event is trigged via IRQ0 or syscall,
STACKINIT_APP EQU (MEMORY_APP_SIZE-4)
STACKINIT_KERNEL_CORE EQU (MEMORY_KERNEL_SIZE/2)

; Should be able to store
; - syscall and all internal calls
; - ISRs and all internal calls
STACKINIT_KERNEL_EVENT EQU (MEMORY_KERNEL_SIZE-4)

