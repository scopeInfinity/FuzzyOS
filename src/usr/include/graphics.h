#pragma once

#define GRAPHICS_MAX_WIDTH   320
#define GRAPHICS_MAX_HEIGHT  200

#define DETECT                  0
#define GRAPHIC_DRIVER_VGA      1

#define GRAPHIC_MODE_320x200x256 0x13

#define  BLACK               0x0
#define  BLUE                0x1
#define  GREEN               0x2
#define  CYAN                0x3
#define  RED                 0x4
#define  MAGENTA             0x5
#define  BROWN               0x6
#define  LIGHT_GRAY          0x7
#define  DARK_GRAY           0x8
#define  LIGHT_BLUE          0x9
#define  LIGHT_GREEN         0xA
#define  LIGHT_CYAN          0xB
#define  LIGHT_RED           0xC
#define  LIGHT_MAGENTA       0xD
#define  YELLOW              0xE
#define  WHITE               0xF

#define SYSCALL_GRAPHICS_INITGRAPH  0
#define SYSCALL_GRAPHICS_CLOSEGRAPH 1
#define SYSCALL_GRAPHICS_COPYBUFFER 2

void initgraph(int *graphdetect, int *graphmode, char *not_used);
int graphresult();
void closegraph();
void cleardevice();

int graphflush();

void setcolor(int color);
int getcolor();
void setbkcolor(int color);
int getbkcolor();

void putpixel_noflush(int x, int y, int color);
void putpixel(int x, int y, int color);

void line(int x1, int y1, int x2, int y2);
void rectangle(int left, int top, int right, int bottom);
void bar(int left, int top, int right, int bottom);
void fillellipse(int xcenter, int ycenter, int x_radius, int y_radius);
