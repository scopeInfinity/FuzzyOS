#include <drivers/display/text_mode.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/disk/disk.h>
#include <lib/utils/logging.h>
#include <lib/utils/output.h>
#include <lib/utils/string.h>
#include <lib/utils/process.h>
#include <lib/utils/input.h>
#include <lib/utils/panic.h>
#include <lib/utils/time.h>
#include <lib/syscall/syscall.h>

#include "kernel/essentials.c"
#include "kernel/interrupts.c"
#include "kernel/process.c"

extern void kernel_enable_interrupts();
extern void kernel_core_entry_asm();

char command[30];
int send_int(int a,int b) {
    asm("int $0x61");
}

int HACK_WAIT = 0;
void kernel_core_entry() {
    set_color_bg(C_BLUE);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    print_log("skipping line...");
    print_log("skipping line...");
    print_log(LOG_PREFIX "Initializing Kernel");

    populate_and_load_idt_table();

    keyboard_init();
    process_handler_init();
    print_log(LOG_PREFIX "Kernel enabling interrupts");

    // Don't even try to use software or hardware int before this STI.
    kernel_enable_interrupts();
    // asm("int $0x08");
    // while(1);

    int need_to_clear_hack = 1;
    while(1) {
        if(need_to_clear_hack) {
            move_xy(0,10);
            print_line("Typewriter: ");
            set_color_bg(C_WHITE);
            set_color_fg(C_BLACK);
            print_rectangle(0, 12, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-2);
            move_xy(0,12);
            print_line("Suppored Commands: run ttt, run calculator, run dashboard, exit\n");
            need_to_clear_hack = 0;
        }
        // strcpy(command, "run ttt");
        read_line(command);
        int run = 0;
        int sector_start, sector_count;
        print_log(LOG_PREFIX "Command: '%s'", command);
        if(strcmpi(command, "run ttt")==0) {
            sector_start = SECTOR_START_APP_TTT;
            sector_count = SECTOR_COUNT_APP_TTT;
            run = 1;
        } else if(strcmpi(command, "run calculator")==0) {
            sector_start = SECTOR_START_APP_CALC;
            sector_count = SECTOR_COUNT_APP_CALC;
            run = 1;
        } else if(strcmpi(command, "run dashboard")==0) {
            sector_start = SECTOR_START_APP_DASHBOARD;
            sector_count = SECTOR_COUNT_APP_DASHBOARD;
            run = 1;
        } else if(strcmpi(command, "exit")==0) {
            PANIC(0, "No Panic, it's a normal exit.");
        }
        if(run)  {
            need_to_clear_hack = 1;
            int exit_code = exec(sector_start, sector_count);
            if(exit_code<0) {
                print_log(LOG_PREFIX "Failed to execute the process.");
            } else {
                print_log(LOG_PREFIX "App exit_code: %d", exit_code);
            }
        } else {
            print_log(LOG_PREFIX "Invalid Command!");
        }
    }
    PANIC(501, "Kernel is under development!!!");
}