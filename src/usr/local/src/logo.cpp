// logo program
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

const int BUSY_WAIT_FRAMECOUNTER = 2e7;

const int WINDOW_HEIGHT = GRAPHICS_MAX_HEIGHT;
const int WINDOW_WIDTH  = GRAPHICS_MAX_WIDTH;

char message_buffer[320];

void frame_wait() {
    // busy wait
    volatile int counter = BUSY_WAIT_FRAMECOUNTER;
    while (counter--);
}

void init() {
    std::graphics::graphautoflush_disable();
}

void draw_screen() {
    // clear screen
    std::graphics::setbkcolor(BLACK);
    std::graphics::cleardevice();

    {
        std::graphics::setcolor(WHITE);
        std::snprintf(message_buffer, sizeof(message_buffer), "LOGO");
        int width = std::graphics::textwidth(message_buffer);
        std::graphics::outtextxy((WINDOW_WIDTH-width)/2, 10, message_buffer);
    }

    std::graphics::graphflush();
}

void logo() {
    while(1) {
        draw_screen();
    }
}

int main(int argc,char *argv[]) {
    int gd = DETECT, gm;
    std::graphics::initgraph(&gd, &gm, NULL);
    int gerr = std::graphics::graphresult();
    if (gerr != 0) {
        std::printf("failed to open graphics mode, err: %d", gerr);
        return 1;
    }
    logo();
    std::graphics::closegraph();
    return 0;
}
