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

int call_main(unsigned short cs,unsigned short ip, int argc, char *argv[]) {
    return ((int (*) (int, char *[]))(unsigned int)ip)(argc, argv);
}

void exec(int sector_index, int sector_count){
    int memory_address = 0x20000;
    int err = load_sectors(memory_address, 0x80, 51, 25);
    if(err) {
        move_xy(3,8);
        print_line("Failed to load app in memory, Error: ");
        print_int(err);
    } else {
        move_xy(2,8);
        print_line("App Loaded: ");
        print_memory_hex(memory_address-KERNEL_MEMORY_LOCATION, 16);
        // call_main(0, memory_address, 0, 0);
        // print_line("App Exited.");
    }
}

void kernel_core_entry() {
    set_color_bg(C_BLUE);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);

    exec(0, 0);

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

    while(1) {
        print_char(getch());
    }
    PANIC(501, "Kernel is under development!!!");
}