#pragma once

#define GRAPHICS_HEIGHT  640
#define GRAPHICS_WIDTH   480

// following functions doesn't verify arguments
inline void draw_pixel(int x, int y, int color);
inline void draw_char(int x, int y, int fnt_size, int color);
// x1<=x2, y1<=y2
inline void draw_rectangle(int x1, int y1, int x2, int y2, int color);
inline void draw_ellipse(int x1, int y1, int x2, int y2, int color);

void graphics_flush();
