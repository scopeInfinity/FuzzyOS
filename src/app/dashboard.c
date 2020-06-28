#include <app/calc.c>
#include <lib/syscalls/io_interface_bios.c>
#include <lib/syscalls/io.h>
#include <lib/syscalls/time.h>
#include <lib/syscalls/color.h>
#include <lib/syscalls/disk.h>
#include <lib/util/string.h>

char query_app_number[] = "Enter Application Number: ";
char application_list[5][15] = {"Calculator", "Sample 2", "Sample 3", "Sample 4", "Sample 5"};
void print_applications(unsigned char x, unsigned char y, char *list, unsigned char count, unsigned char max_strlen) {
    for(int i=1;i<=count;i++) {
        move_xy(x,y+i);
        set_color_fg(C_BLUE);
        print_int(i);
        print_line(" ");
        set_color_fg(C_BLACK);
        print_line(list);
        list+=max_strlen;
    }
}

void print_board() {
    set_color_bg(C_DARK_GRAY);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, WINDOW_WIDTH-1, WINDOW_HEIGHT-1);
    move_xy(1,0);
    print_line("Fuzzy OS");

    set_color_bg(C_LIGHT_GRAY);
    set_color_fg(C_BLACK);
    print_rectangle(1, 1, WINDOW_WIDTH-2, WINDOW_HEIGHT-2);
    move_xy(3,3);

    print_line(query_app_number);
    print_applications(4,4, (char*)application_list, 5, 15);
}

int call_main(unsigned short cs,unsigned short ip, int argc, char *argv[]) {
    return ((int (*) (int, char *[]))(unsigned int)ip)(argc, argv);
}

int run_dashboard(int argc,char **argv) {
    while(1) {
        print_board();
        move_xy(3+sizeof(query_app_number)-1,3);
        int num = read_int();

        // Load and launch calculator
        // int err = load_sectors(0xC000, 0x80, 27, 25);
        int err = load_sectors(0xC000, 0x80, 52, 25);
        if(err) {
            set_color_bg(C_DARK_GRAY);
            move_xy(2,WINDOW_HEIGHT-1);
            print_line("Failed to load app in memory, Error: ");
            print_int(err);
            sleep(1000);
            continue;
        }
        char *argv[] = {"argv1, argv2", "argv3"};
        int return_status = call_main(0x0C00,0x0000, 3, argv);

        print_board();
        set_color_bg(C_DARK_GRAY);
        move_xy(2,WINDOW_HEIGHT-1);
        print_line("Program Exited: ");
        print_int(return_status);
        sleep(1000);
    }
    return 0;
}