#include <drivers/display/text_mode.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/disk/disk.h>
#include <lib/utils/logging.h>
#include <lib/utils/output.h>
#include <lib/utils/input.h>
#include <lib/utils/panic.h>
#include <lib/utils/time.h>
#include <lib/syscall/syscall.h>

#include "kernel/essentials.c"
#include "kernel/interrupts.c"
#include "kernel/process.c"

extern void kernel_enable_interrupts();

char command[30];
int send_int(int a,int b) {
    asm("int $0x61");
}

void kernel_core_entry() {
    set_color_bg(C_BLUE);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    print_log("Initializing Kernel");

    populate_and_load_idt_table();
    print_log("Kernel enabling interrupts");
    kernel_enable_interrupts();
    keyboard_init();

    process_handler_init();
    int need_to_clear_hack = 1;
    while(1) {
        if(need_to_clear_hack) {
            move_xy(0,10);
            print_line("Typewriter: ");
            set_color_bg(C_WHITE);
            set_color_fg(C_BLACK);
            print_rectangle(0, 12, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-2);
            move_xy(0,12);
            print_line("Suppored Commands: run ttt, run calculator, exit\n");
            need_to_clear_hack = 0;
        }
        read_line(command);
        int run = 0;
        int sector_start, sector_count;
        print_log("Command: '%s'", command);
        if(strcmpi(command, "run ttt")==0) {
            sector_start = SECTOR_START_APP_TTT;
            sector_count = SECTOR_COUNT_APP_TTT;
            run = 1;
        } else if(strcmpi(command, "run calculator")==0) {
            sector_start = SECTOR_START_APP_CALC;
            sector_count = SECTOR_COUNT_APP_CALC;
            run = 1;
        } else if(strcmpi(command, "exit")==0) {
            PANIC(0, "No Panic, it's a normal exit.");
        }

        if(run)  {
            need_to_clear_hack = 1;
            int exit_code = syscall(1, sector_start, sector_count, 0,0);
            if(exit_code<0) {
                print_log("Failed to execute the process.");
            } else {
                print_log("App exit_code: %d", exit_code);
            }
        } else {
            print_log("Invalid Command!");
        }
    }
    PANIC(501, "Kernel is under development!!!");
}