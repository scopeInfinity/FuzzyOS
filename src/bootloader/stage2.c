#include <app/dashboard.c>
#include <lib/syscalls/io.h>
#include <lib/syscalls/time.h>
#include <lib/syscalls/color.h>

char message_welcome[] = "C says 'Hello World'";
char message_dashboard[] = "Opening App 'Dashboard'";

void entry_stage() {
    move_xy(6, 11);
    set_color_bg(C_BLACK);
    set_color_fg(C_GREEN);
    print_line(message_welcome);
    set_color_fg(C_WHITE);
    move_xy(6, 13);
    print_line(message_dashboard);
    sleep(100);
    run_dashboard(0,0);
    while(1);
}