// Simple Calculator
#include <stdio.h>
#include <string.h>
#include <conio.h>

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
        printf("   %c|%c|%c\n", s->mat[i][0], s->mat[i][1], s->mat[i][2]);
        if(i<2) {
            printf("   -----\n");
        }
    }
}

void redraw(struct State *s) {
    clrscr();
    gotoxy(0, 0);
    puts("TicTacToe\n");
    puts("---------\n");
    puts("\n");
    puts("Player 1 : X\n");
    puts("Player 2 : O\n");
    puts("\n");
    printf("Turn: Player %d", s->turn+1);
    puts("\n");
    puts("Controls: Use 1-9 in numpad keys pattern\n");
    puts("        : R to reset game\n");
    puts("        : Q to quit game\n\n");
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
                printf("KeyPressed: %d\n", c);
            }
        }
    }
    return 0;
}