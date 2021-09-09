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
    *graphmode = GRAPHIC_MODE_320x200x256;
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

int graphflush() {
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
    return GRAPHICS_MAX_WIDTH-1;
}

int getmaxy() {
    return GRAPHICS_MAX_HEIGHT-1;
}

void setcolor(int color) {
    gstate.color = color;
}

inline int getcolor() {
    return gstate.color;
}

inline void setbkcolor(int color) {
    gstate.bkcolor = color;
}

int getbkcolor() {
    return gstate.bkcolor;
}

void putpixel(int x, int y, int color) {
    BUFFER[y][x]=color;
    graphflush();
}

void putpixel_noflush(int x, int y, int color) {
    BUFFER[y][x]=color;
}

void line(int x1, int y1, int x2, int y2) {
    const int color = getcolor();
    if(abs(x1-x2)>=abs(y1-y2)) {
        // horizontal length is longer
        if(x1>x2) {
            int t;
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        // x1<=x2
        const int xdiff = x2-x1;
        const int ydiff = y2-y1;
        for (int x = x1; x <= x2; x++) {
            int y = ydiff*(x-x1)/xdiff;
            BUFFER[y][x]=color;
        }
    } else  {
        // vertical length is longer
        if(y1>y2) {
            int t;
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        // y1<=y2
        const int xdiff = x2-x1;
        const int ydiff = y2-y1;
        for (int y = y1; y <= y2; y++) {
            int x = y*xdiff/ydiff+x1;
            BUFFER[y][x]=color;
        }
    }
    graphflush();
}

void drawpoly(int num, int polypoints[]) {
    // TODO
}

void filpoly(int num, int polypoints[]) {
    // TODO: ??? how
}

void rectangle(int left, int top, int right, int bottom) {
    const int color = getcolor();

    for(int x=left;x<=right;x++) {
        BUFFER[top][x]=color;
        BUFFER[bottom][x]=color;
    }
    for(int y=top+1;y<bottom;y++) {
        BUFFER[y][left]=color;
        BUFFER[y][right]=color;
    }
    graphflush();
}

void bar(int left, int top, int right, int bottom) {
    const int color = getcolor();

    for(int y=top;y<=bottom;y++) {
        for(int x=left;x<=right;x++) {
            BUFFER[y][x]=color;
        }
    }
    graphflush();
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