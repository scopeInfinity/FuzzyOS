#include <drivers/display/text_mode.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/disk/disk.h>
#include <lib/utils/output.h>
#include <lib/utils/input.h>
#include <lib/utils/panic.h>
#include <lib/utils/time.h>

#include "kernel/essentials.c"
#include "kernel/interrupts.c"

extern void kernel_enable_interrupts();

extern int call_main(int argc, char *argv[]);

void exec(int sector_index, int sector_count){
    int err = load_sectors(MEMORY_LOCATION_APP, 0x80, SECTOR_START_APP_CALC, SECTOR_COUNT_APP_CALC);
    if(err) {
        move_xy(3,8);
        print_line("Failed to load app in memory, Error: ");
        print_int(err);
    } else {
        move_xy(2,8);
        print_line("App Loaded: ");
        print_memory_hex((char*)(MEMORY_LOCATION_APP-MEMORY_LOCATION_KERNEL), 16);
        int exit_code = call_main(0, 0);
        print_line("App Exited: ");
        print_int(exit_code);
    }
}

void kernel_core_entry() {
    set_color_bg(C_BLUE);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);


    move_xy(2,2);
    print_line("Initializing Kernel...");
    populate_and_load_idt_table();
    kernel_enable_interrupts();
    keyboard_init();

    move_xy(2,10);
    print_line("Keyboard: ");

    set_color_bg(C_WHITE);
    set_color_fg(C_BLACK);
    print_rectangle(0, 12, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-2);
    move_xy(0,12);

    exec(0, 0);
    while(1) {
        print_char(getch());
    }
    PANIC(501, "Kernel is under development!!!");
}