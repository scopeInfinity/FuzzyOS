
#include <fuzzy/fs/ffs.h>
#include <fuzzy/kernel/interrupts/interrupts.h>
#include <fuzzy/kernel/interrupts/timer.h>
#include <fuzzy/kernel/process/process.h>

#include <string.h>
#include <process.h>
#include <sys/syscall.h>
#include <conio.h>

#include <drivers/disk/disk.h>
#include <drivers/display/text_mode.h>
#include <drivers/keyboard/keyboard.h>

#include <lib/utils/input.h>
#include <lib/utils/logging.h>
#include <lib/utils/output.h>
#include <fuzzy/kernel/panic.h>
#include <lib/utils/time.h>

extern void kernel_enable_interrupts();
extern void kernel_core_entry_asm();

char command[30];
int need_to_clear_hack;
int run;
int lba_start, sector_count;
extern void enable_timer_interrupt();

void kernel_core_entry() {
    set_color_bg(C_BLUE);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    print_log("Initializing Kernel");


    populate_and_load_idt_table();

    kernel_core_entry_asm();

    print_log("Kernel enabling interrupts");

    kernel_enable_interrupts();
    keyboard_init();

    process_scheduler_init();

    clrscr();

    int init_pid = spawn(INIT_APPNAME);
    print_log("init process created: %d", init_pid);

    // interrupt_pit_enable();
    while (1);
}