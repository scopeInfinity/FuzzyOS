// ping pong game
#include <conio.h>
#include <graphics.h>
#include <stdio.h>

#define DIR_UP -1
#define DIR_DOWN 1
#define PLAYER_1 1
#define PLAYER_2 2

const int BUSY_WAIT_FRAMECOUNTER = 2e7;

const int WINDOW_HEIGHT = GRAPHICS_MAX_HEIGHT;
const int WINDOW_WIDTH = GRAPHICS_MAX_WIDTH;

const int BORDER = 5;

const int BAT_HALF_HEIGHT = 15;
const int BAT_HALF_WIDTH = 2;
const int BAT_SPEED = 5;

const int p1_x = 10;
const int p2_x = (WINDOW_WIDTH - 1) - p1_x;

const float BALL_SPEED_MAX = 10;
const float BALL_SPEED_INIT = 5;
const float BALL_SPEED_INC = 1.1;
const int BALL_RADIUS = 3;

char message_buffer[320];

struct playerState {
    float y;
    float yspeed;
};

struct {
    int top, bottom;
    int left, right;
} field;

struct {
    float x, y, xprev, yprev;
    float xspeed, yspeed;
} ball;

struct {
    struct playerState p1, p2;
    int turn;
    int p1Score;
    int p2Score;
} gstate;

void frame_wait() {
    // busy wait
    volatile int counter = BUSY_WAIT_FRAMECOUNTER;
    while (counter--)
        ;
}

void reset_ball() {
    ball.xprev = ball.x = WINDOW_WIDTH / 2;
    ball.yprev = ball.y = WINDOW_HEIGHT / 2;
    if (gstate.turn == PLAYER_1) {
        ball.xspeed = BALL_SPEED_INIT * 0.8;
        ball.yspeed = BALL_SPEED_INIT * 0.6;
    } else {
        ball.xspeed = -BALL_SPEED_INIT * 0.8;
        ball.yspeed = BALL_SPEED_INIT * 0.6;
    }
}

void game_init() {
    graphautoflush_disable();

    field.top = BORDER - 1;
    field.bottom = WINDOW_HEIGHT - BORDER;
    field.left = BORDER - 1;
    field.right = WINDOW_WIDTH - BORDER;

    gstate.p1.y = (field.top + field.bottom) / 2;
    gstate.p2 = gstate.p1;

    gstate.p1Score = gstate.p2Score = 0;
    gstate.turn = PLAYER_1;
    reset_ball();
}

void check_collision() {
    // wall

    if (ball.y > field.bottom - BALL_RADIUS) {
        ball.yspeed *= -1;
        ball.y = 2 * (field.bottom - BALL_RADIUS) - ball.y;
    } else if (ball.y < field.top + BALL_RADIUS) {
        ball.yspeed *= -1;
        ball.y = 2 * (field.top + BALL_RADIUS) - ball.y;
    }

    if (ball.x > p2_x - BAT_HALF_WIDTH - BALL_RADIUS &&
        ball.y >= gstate.p2.y - BAT_HALF_HEIGHT &&
        ball.y <= gstate.p2.y + BAT_HALF_HEIGHT) {
        // right bat
        ball.xspeed *= -1 * BALL_SPEED_INC;
        ball.x = 2 * (p2_x - BAT_HALF_WIDTH - BALL_RADIUS) - ball.x;
    } else if (ball.x < p1_x + BAT_HALF_WIDTH + BALL_RADIUS &&
               ball.y >= gstate.p1.y - BAT_HALF_HEIGHT &&
               ball.y <= gstate.p1.y + BAT_HALF_HEIGHT) {
        // left bat
        ball.xspeed *= -1 * BALL_SPEED_INC;
        ball.x = 2 * (p1_x + BAT_HALF_WIDTH + BALL_RADIUS) - ball.x;
    }

    if (ball.x > WINDOW_WIDTH - 1) {
        // player 2 missed ball
        gstate.p1Score += 1;
        gstate.turn = (gstate.turn == PLAYER_1) ? PLAYER_2 : PLAYER_1;
        reset_ball();
    } else if (ball.x < 0) {
        // player 1 missed ball
        gstate.p2Score += 1;
        gstate.turn = (gstate.turn == PLAYER_1) ? PLAYER_2 : PLAYER_1;
        reset_ball();
    }
}

void draw_board() {
    // clear screen
    setbkcolor(GREEN);
    cleardevice();

    setcolor(BROWN);
    bar(0, 0, WINDOW_WIDTH, field.top);
    bar(0, field.bottom, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);

    {
        setcolor(BLACK);
        snprintf(message_buffer, sizeof(message_buffer), "%d | score | %d",
                 gstate.p1Score, gstate.p2Score);
        int width = textwidth(message_buffer);
        outtextxy((WINDOW_WIDTH - width) / 2, field.top + 10, message_buffer);

        snprintf(message_buffer, sizeof(message_buffer),
                 "Control: w/s | p/l ; Exit: q");
        width = textwidth(message_buffer);
        outtextxy((WINDOW_WIDTH - width) / 2, field.bottom - 15,
                  message_buffer);
    }

    setcolor(BLUE);
    bar(p1_x - BAT_HALF_WIDTH, gstate.p1.y - BAT_HALF_HEIGHT,
        p1_x + BAT_HALF_WIDTH, gstate.p1.y + BAT_HALF_HEIGHT);

    setcolor(RED);
    bar(p2_x - BAT_HALF_WIDTH, gstate.p2.y - BAT_HALF_HEIGHT,
        p2_x + BAT_HALF_WIDTH, gstate.p2.y + BAT_HALF_HEIGHT);

    setcolor(WHITE);
    fillellipse(ball.x, ball.y, BALL_RADIUS, BALL_RADIUS);

    graphflush();
}

void move_bat(int player_id, int dir) {
    struct playerState *p = NULL;
    if (player_id == PLAYER_1) {
        p = &gstate.p1;
    } else if (player_id == PLAYER_2) {
        p = &gstate.p2;
    } else {
        return;
    }

    if (dir == DIR_UP) {
        p->y -= BAT_SPEED;
    } else if (dir == DIR_DOWN) {
        p->y += BAT_SPEED;
    }

    // keep bat in bounds
    const int bat_miny = field.top + 1 + BAT_HALF_HEIGHT;
    const int bat_maxy = field.bottom - 1 - BAT_HALF_HEIGHT;

    if (p->y > bat_maxy) {
        p->y = bat_maxy;
    } else if (p->y < bat_miny) {
        p->y = bat_miny;
    }
}

void move_ball() {
    ball.xprev = ball.x;
    ball.yprev = ball.y;
    ball.x += ball.xspeed;
    ball.y += ball.yspeed;
}

void game() {
    game_init();
    while (1) {
        check_collision();
        draw_board();

        move_ball();

        frame_wait();
        if (!kbhit()) {
            continue;
        }
        char c = getch();
        if (c == 'q') {
            return;
        } else if (c == 'w') {
            move_bat(PLAYER_1, DIR_UP);
        } else if (c == 's') {
            move_bat(PLAYER_1, DIR_DOWN);
        } else if (c == 'p') {
            move_bat(PLAYER_2, DIR_UP);
        } else if (c == 'l') {
            move_bat(PLAYER_2, DIR_DOWN);
        }
    }
}

int main(int argc, char *argv[]) {
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
