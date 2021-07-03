#include <lib/utils/color.h>
#include <lib/utils/output.h>

void entry_console_init() {
    set_color_bg(C_BLACK);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    move_xy(0,0);
}