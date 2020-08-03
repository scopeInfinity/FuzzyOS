#include <drivers/display/text_mode.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/disk/disk.h>
#include <lib/utils/logging.h>
#include <lib/utils/output.h>
#include <lib/utils/input.h>
#include <lib/utils/panic.h>
#include <lib/utils/time.h>

#include "kernel/essentials.c"
#include "kernel/interrupts.c"

extern void kernel_enable_interrupts();

extern int call_main(int argc, char *argv[]);

void exec(int sector_index, int sector_count){
    print_log("Exec is loading app in memory.");

    int err = load_sectors(MEMORY_LOCATION_APP, 0x80, SECTOR_START_APP_TTT, SECTOR_COUNT_APP_TTT);
    if(err) {
        print_log("Failed to load app in memory, Error: ", err);
    } else {
        int relative_address = MEMORY_LOCATION_APP-MEMORY_LOCATION_KERNEL;
        print_log("App loaded at 0x%x, relative_address: 0x%x: %x...",
            MEMORY_LOCATION_APP, relative_address,
            *(int*)relative_address);
        int exit_code = call_main(0, 0);
        print_log("App exit_code: %d", exit_code);
    }
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

    move_xy(0,10);
    print_line("Typewriter: ");
    set_color_bg(C_WHITE);
    set_color_fg(C_BLACK);
    print_rectangle(0, 12, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-2);
    move_xy(0,12);
    while(1) {
        print_char(getch());
    }
    exec(0, 0);
    PANIC(501, "Kernel is under development!!!");
}