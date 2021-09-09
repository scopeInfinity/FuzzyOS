// ping pong game
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

#define DIR_UP   -1
#define DIR_DOWN  1
#define PLAYER_1  1
#define PLAYER_2  2

const int WINDOW_HEIGHT = GRAPHICS_MAX_HEIGHT;
const int WINDOW_WIDTH  = GRAPHICS_MAX_WIDTH;

const int BAT_HALF_HEIGHT = 15;
const int BAT_HALF_WIDTH  = 2;
const int BAT_SPEED = 5;

const int bat_miny = BAT_HALF_HEIGHT;
const int bat_maxy = (WINDOW_HEIGHT-1)-BAT_HALF_HEIGHT;

const int p1_x = 10;
const int p2_x = (WINDOW_WIDTH-1)-p1_x;

const int BALL_SPEED = 2;
const int BALL_RADIUS = 3;

struct playerState {
    int y;
};

struct {
    int x,y;
    int xspeed, yspeed;
} ball;

struct {
    struct playerState p1, p2;
} gstate;

void frame_wait() {
    // busy wait
    volatile int counter = 1e7;
    while (counter--);
}

void game_init() {
    gstate.p1.y = (bat_miny+bat_maxy)/2;
    gstate.p2 = gstate.p1;

    ball.x = WINDOW_WIDTH/2;
    ball.y = WINDOW_HEIGHT/2;
    ball.xspeed = BALL_SPEED*0.8;
    ball.yspeed = BALL_SPEED*0.6;
}

void draw_board() {
    // clear screen
    setbkcolor(GREEN);
    cleardevice();

    setcolor(BLUE);
    bar(p1_x-BAT_HALF_WIDTH, gstate.p1.y-BAT_HALF_HEIGHT,
        p1_x+BAT_HALF_WIDTH, gstate.p1.y+BAT_HALF_HEIGHT);

    setcolor(RED);
    bar(p2_x-BAT_HALF_WIDTH, gstate.p2.y-BAT_HALF_HEIGHT,
        p2_x+BAT_HALF_WIDTH, gstate.p2.y+BAT_HALF_HEIGHT);

    setcolor(WHITE);
    fillellipse(ball.x, ball.y, BALL_RADIUS, BALL_RADIUS);
}

void move_bat(int player_id, int dir) {
    struct playerState* p = NULL;
    if(player_id==PLAYER_1) {
        p = &gstate.p1;
    } else if(player_id==PLAYER_2) {
        p = &gstate.p2;
    } else {
        return;
    }

    if(dir==DIR_UP) {
        p->y -= BAT_SPEED;
    } else if(dir==DIR_DOWN) {
        p->y += BAT_SPEED;
    }

    // keep bat in bounds
    p->y = max(bat_miny, min(bat_maxy, p->y));
}

void move_ball() {
    ball.x += ball.xspeed;
    ball.y += ball.yspeed;
}

void game() {
    game_init();
    while (1)  {
        draw_board();

        move_ball();

        if(!kbhit()) {
            frame_wait();
            continue;
        }
        char c = getch();
        if(c=='e') {
            return;
        } else if(c=='w') {
            move_bat(PLAYER_1, DIR_UP);
        } else if(c=='s') {
            move_bat(PLAYER_1, DIR_DOWN);
        } else if(c=='p') {
            move_bat(PLAYER_2, DIR_UP);
        } else if(c=='l') {
            move_bat(PLAYER_2, DIR_DOWN);
        }
    }
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