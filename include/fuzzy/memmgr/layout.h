#pragma once

// START_ENSURE_SAME_layout_asm
#define MEMORY_APP_SIZE         0x20000
#define STACKINIT_APP           (MEMORY_APP_SIZE-4)
// END_ENSURE_SAME_layout_asm

// Most of the memory layout relies on the fact the kernel is first app
// and kernel's pid is 0.
#define PID_KERNEL 0

/*
 * Bug:
 *  Bootloader stage 2 is not able to load kernel if
 *  memory address need 20 address lines (0x10000) in
 *  our case.
 *  It works as intended in QEMU but not in others.
 * Mitigation:
 *  - Forcing MEMORY_KERNEL_LOCATION to be represented
 *    by 16 address lines. For ex 0xF000.
 * Resolution:
 *  - I'm not sure.
 */
#define MEMORY_APPBASE_LOCATION 0x10000

#define memmgr_app_abs_location(pid) ((pid==0)?0x0F000:(MEMORY_APPBASE_LOCATION + (pid)*MEMORY_APP_SIZE))
#define memmgr_app_size(pid) (MEMORY_APP_SIZE)

#define MEMORY_KERNEL_LOCATION (memmgr_app_abs_location(PID_KERNEL))
#define MEMORY_KERNEL_SIZE (memmgr_app_size(PID_KERNEL))
