// ping pong game
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

void game() {
    // clear screen
    setbkcolor(WHITE);
    cleardevice();
    getch();
}

int main(int argc,char *argv[]) {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    int gerr = graphresult();
    if (gerr != 0) {
        printf("failed to open graphics mode, err: %d", gerr);
        return 1;
    }
    game();
    closegraph();
    return 0;
}