#include <drivers/display/text_mode.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/essentials.c>
#include <lib/utils/io.h>
#include <lib/utils/panic.h>

int call_main(unsigned short cs,unsigned short ip, int argc, char *argv[]) {
    return ((int (*) (int, char *[]))(unsigned int)ip)(argc, argv);
}

void exec(int sector_index, int sector_count){
    int memory_address = 0x2000;
    int err = 0;
    // Load Sectors for proctected mode is not implemented yet.
    // int err = load_sectors(memory_address, 0x80, 52, 25);
    if(err) {
        move_xy(3,4);
        print_line("Failed to load app in memory, Error: ");
        print_int(err);
    } else {
        call_main(0, memory_address, 0, 0);
        print_line("App Exited.");
    }
}

void entry_core() {
    set_color_bg(C_BLACK);
    set_color_fg(C_WHITE);

    move_xy(6,22);
    print_line("Initializing Kernel...");
    // keyboard_init();
    PANIC(501, "Kernel is under development!!!");
}