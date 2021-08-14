
#include <fuzzy/fs/ffs.h>
#include <fuzzy/kernel/interrupts/interrupts.h>
#include <fuzzy/kernel/interrupts/timer.h>
#include <fuzzy/kernel/process/process.h>

#include <string.h>
#include <process.h>
#include <sys/syscall.h>

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

    int syscall_out = -1;
    syscall_out = syscall(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_LBA_SC, SECTOR_START_APP_SH, SECTOR_COUNT_APP_SH, 0);
    syscall_out = syscall(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_LBA_SC, SECTOR_START_APP_LS, SECTOR_COUNT_APP_LS, 0);
    print_log("Create process: %d", syscall_out);
    interrupt_pit_enable();
    while (1);

    need_to_clear_hack = 1;
    while(1) {
        if(need_to_clear_hack) {
            print_log("Typewriter: ");
            print_log("Suppored Commands: run ttt, run calculator, exit");
            need_to_clear_hack = 0;
        }
        run = 0;
        // broken, using RUN_APP_ID for now
        command[0]='\0';
        print_log("Command: '%s'", command);
        if(RUN_APP_ID == 1 || strcmpi(command, "run ttt")==0) {
            lba_start = SECTOR_START_APP_TTT;
            sector_count = SECTOR_COUNT_APP_TTT;
            run = 1;
        } else if(RUN_APP_ID == 2 || strcmpi(command, "run calculator")==0) {
            lba_start = SECTOR_START_APP_CALC;
            sector_count = SECTOR_COUNT_APP_CALC;
            run = 1;
        } else if(RUN_APP_ID == 3 || strcmpi(command, "run ls")==0) {
            lba_start = SECTOR_START_APP_LS;
            sector_count = SECTOR_COUNT_APP_LS;
            run = 1;
        } else if(RUN_APP_ID == 4 || strcmpi(command, "run cat")==0) {
            lba_start = SECTOR_START_APP_CAT;
            sector_count = SECTOR_COUNT_APP_CAT;
            run = 1;
        } else if(RUN_APP_ID == 5 || strcmpi(command, "run sh")==0) {
            lba_start = SECTOR_START_APP_SH;
            sector_count = SECTOR_COUNT_APP_SH;
            run = 1;
        } else if(strcmpi(command, "exit")==0) {
            PANIC(0, "No Panic, it's a normal exit.");
        }

        if(run)  {
            need_to_clear_hack = 1;
            int exit_code = syscall(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_SPAWN_LBA_SC, lba_start, sector_count, 0);
            if(exit_code<0) {
                PANIC(exit_code, "Failed to execute the process.");
            } else {
                print_log("App exit_code: %d", exit_code);
            }
        } else {
            print_log("Invalid Command!");
        }
    }
    PANIC(501, "Kernel is under development!!!");
}