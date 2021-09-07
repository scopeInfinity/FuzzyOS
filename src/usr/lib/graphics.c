#include <graphics.h>
#include <string.h>
#include <sys/syscall.h>

static struct GraphicsState {
    int bkcolor;
    int color;
    int last_err;
} gstate = {0};

static uint8_t BUFFER[GRAPHICS_MAX_HEIGHT][GRAPHICS_MAX_WIDTH]={0};

void initgraph(int *graphdetect, int *graphmode, char *not_used) {
    if (*graphdetect == DETECT) {
        *graphdetect = GRAPHIC_DRIVER_VGA;
    }
    if (*graphdetect != GRAPHIC_DRIVER_VGA) {
        // only one graphics driver is supported for now.
        return -4;
    }
    *graphmode = GRAPHIC_MODE_320x200x16;
    gstate.last_err = SYSCALL_A1(SYSCALL_GRAPHICS, SYSCALL_GRAPHICS_INITGRAPH);
}

int graphresult() {
    int err = gstate.last_err;
    gstate.last_err = 0;
    return err;
}

void closegraph() {
    gstate.last_err = SYSCALL_A1(SYSCALL_GRAPHICS, SYSCALL_GRAPHICS_CLOSEGRAPH);
}

static int graphflush() {
    gstate.last_err = SYSCALL_A2(SYSCALL_GRAPHICS, SYSCALL_GRAPHICS_COPYBUFFER, BUFFER);
    return gstate.last_err;
}

void cleardevice() {
    memset(BUFFER, gstate.bkcolor, sizeof(BUFFER));
    moveto(0, 0);
    graphflush();
}

void setviewport(int left, int top, int right, int bottom, int clip) {
    // TODO
}

int getmaxx() {
    // TODO
}

int getmaxy() {
    // TODO
}

void setcolor(int color) {
    // TODO
}

int getcolor() {
    // TODO
}

void setbkcolor(int color) {
    // TODO
}

int getbkcolor() {
    // TODO
}

void putpixel(int x, int y, int color) {
    // TODO
}

void line(int x1, int y1, int x2, int y2) {
    // TODO
}

void drawpoly(int num, int polypoints[]) {
    // TODO
}

void filpoly(int num, int polypoints[]) {
    // TODO: ??? how
}

void rectangle(int left, int top, int right, int bottom) {
    line(left, top, right, top);
    line(left, bottom, right, bottom);
    line(left, top, left, bottom);
    line(right, top, right, bottom);
}

void bar(int left, int top, int right, int bottom) {
    // filled rectangle
    // TODO
}

void ellipse(int x, int y, int x_radius, int y_radius) {
    // TODO
}

void fillellipse(int x, int y, int x_radius, int y_radius) {
    // TODO
}

void floodfill(int x,int y, int color) {
    // TODO
}

// For text, later
int textheight(char *str) {
    // TODO
}

int textwidth(char *str) {
    // TODO
}

void outtext(char *str) {
    // TODO
}

void outtextxy(int x, int y, char *str) {
    // TODO
}

void moveto(int x, int y) {
    // TODO
}

int getx() {
    // TODO
}

int gety() {
    // TODO
}