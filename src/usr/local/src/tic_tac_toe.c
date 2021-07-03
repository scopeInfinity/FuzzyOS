// Simple Calculator
#define BUILD_FOR_FUZZY
#ifndef BUILD_FOR_FUZZY
#else
#include <lib/utils/input.h>
#include <lib/utils/output.h>
#include <lib/utils/time.h>
#include <lib/utils/string.h>
#endif

void console_init() {
    set_color_bg(C_BLACK);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    move_xy(0,0);
}

struct State
{
    unsigned char turn; // 0 or 1
    unsigned char mat[3][3];
};

void reset(struct State *s) {
    s->turn = 0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            s->mat[i][j]=' ';
}

void print_board(struct State *s) {
    for (int i = 0; i < 3; ++i) {
        print_line("   ");
        print_char(s->mat[i][0]);
        print_char('|');
        print_char(s->mat[i][1]);
        print_char('|');
        print_char(s->mat[i][2]);
        print_char('\n');
        if(i<2) {
            print_line("   -----\n");
        }
    }
}

void redraw(struct State *s) {
    console_init();
    print_line("TicTacToe\n");
    print_line("---------\n");
    print_line("\n");
    print_line("Player 1 : X\n");
    print_line("Player 2 : O\n");
    print_line("\n");
    print_line("Turn: Player ");
    print_int(s->turn+1);
    print_line("\n");
    print_line("Controls: Use 1-9 in numpad keys pattern\n");
    print_line("        : R to reset game\n");
    print_line("        : Q to quit game\n\n");
    print_board(s);

    // __TEST_INJECT_APP_TTT_ENTRY__: __asm__("hlt");
}

void play_move(struct State *s, unsigned char r, unsigned char c) {
    if(s->mat[r][c]!=' ')
        return;
    char mark = 'X';
    if (s->turn==1) {
        mark = 'O';
    }
    s->mat[r][c]=mark;
    s->turn=1-(s->turn);
}

struct State s;
unsigned char quit;
unsigned char greset;
int main(int argc,char *argv[]) {
    greset = 1;
    quit = 0;
    while(1) {
        if(quit) break;
        if(greset) {
            reset(&s);
            greset = 0;
        }
        redraw(&s);
        while(1){
            char c = getch();
            if(c=='r' || c=='R') {
                greset = 1;
                break;
            }
            if(c=='q' || c=='Q') {
                quit = 1;
                break;
            }
            char row = -1, col = -1;
            if(c=='1' || c=='2' || c=='3') row = 2;
            if(c=='4' || c=='5' || c=='6') row = 1;
            if(c=='7' || c=='8' || c=='9') row = 0;
            if(c=='1' || c=='4' || c=='7') col = 0;
            if(c=='2' || c=='5' || c=='8') col = 1;
            if(c=='3' || c=='6' || c=='9') col = 2;
            if(row>=0) {
                play_move(&s, row, col);
                break;
            } else {
                print_int(c);
            }
        }
    }
    return 0;
}