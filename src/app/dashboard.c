#include <app/calc.c>
#include <lib/syscalls/io.h>
#include <lib/syscalls/time.h>
#include <lib/syscalls/color.h>
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

void console_init() {
    set_color_bg(C_BLACK);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    move_xy(0,0);
}

int run_dashboard(int argc,char **argv) {
    while(1) {
        set_color_bg(C_DARK_GRAY);
        set_color_fg(C_WHITE);
        print_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        move_xy(1,0);
        print_line("Fuzzy OS");
        
        set_color_bg(C_LIGHT_GRAY);
        set_color_fg(C_BLACK);
        print_rectangle(1, 1, WINDOW_WIDTH-1, WINDOW_HEIGHT-1);
        move_xy(3,3);
        
        print_line(query_app_number);
        print_applications(4,4, (char*)application_list, 5, 15);
        move_xy(3+sizeof(query_app_number)-1,3);
        int num = read_int();
        console_init();
        calc_main(0,0);
    }
    return 0;
}