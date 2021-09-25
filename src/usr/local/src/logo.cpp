// logo program
#include <iostream.h>
#include <utility.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

const int BUSY_WAIT_FRAMECOUNTER = 2e7;

const int WINDOW_HEIGHT = GRAPHICS_MAX_HEIGHT;
const int WINDOW_WIDTH  = GRAPHICS_MAX_WIDTH;
const int INTERPRETER_HEIGHT = 16*4;

char message_buffer[320];

void frame_wait() {
    // busy wait
    volatile int counter = BUSY_WAIT_FRAMECOUNTER;
    while (counter--);
}
class Display {
    int height;
    typedef std::pair<int,int> Point;  // x, y
    #define _x first
    #define _y second
    std::vector<std::pair<Point, Point> > lines;
    // logo
    Point current;
public:
    Display() : current(WINDOW_WIDTH/2, WINDOW_HEIGHT/2) {
    }

    void set_height(int height) {
        this->height = height;
        this->current._y = this->height/2;
    }

    int parse(std::string input) {
        if (input == "fd") {
            Point next = current;
            next._y += 100;
            lines.push_back(std::make_pair(current, next));
            current = next;
            return 0;
        }
        return 1;
    }

    void draw() {
        std::graphics::setcolor(WHITE);
        std::graphics::bar(0, 0, WINDOW_WIDTH-1, this->height-1);
        std::graphics::setcolor(BLACK);

        for(const auto &line: lines) {
            std::graphics::line(line.first.first, line.first.second,
                                line.second.first, line.second.second);
        }
        // TODO: draw turtle
        std::graphics::graphflush();
    }
    #undef _x
    #undef _y
};

class Console {
    const int line_count = 4;
    std::vector<std::string> data;
    int y_offset;
    int text_height;
    const int border = 5;

public:
    Console(): data(line_count, "") {
        text_height = std::graphics::textheight(".");
        y_offset = WINDOW_HEIGHT-text_height*line_count-2*border;
    }

    int get_y_offset() {
        return this->y_offset;
    }

    std::string read() {
        int index = 0;
        std::string &input = data[index];
        char ch;
        while(1) {
            ch = std::getch();
            if(ch=='\n') {
                std::string out = input;  // copy
                input.clear();
                draw();
                return out;
            }
            input+=ch;
            draw();
        }
    }

    void set_err(int err) {
        int index = 1; // error line
        if(err==0) {
            data[index].clear();
        } else {
            data[index] = "error!";
        }
        draw();
    }
    void draw() {
        std::graphics::setcolor(YELLOW);
        std::graphics::bar(0, y_offset, WINDOW_WIDTH-1, WINDOW_HEIGHT-1);
        std::graphics::setcolor(BLACK);
        for (std::size_t i = 0; i < line_count; i++) {
            std::graphics::outtextxy(border, y_offset+i*text_height+border, data[i].c_str());
        }
        std::graphics::graphflush();
    }
};

class Interpreter {
    Display display;
    Console console;
public:
    Interpreter() {
        display.set_height(console.get_y_offset());
    }
    void execute() {
        display.draw();
        console.draw();
        while(1) {
            std::string input = console.read();
            int err = display.parse(input);
            console.set_err(err);
            if(!err) {
                display.draw();
                console.draw();
            }
        }
    }
};

void start_logo() {
    std::graphics::graphautoflush_disable();
    std::graphics::setbkcolor(BLACK);

    Interpreter interpreter;
    interpreter.execute();
}

void cleanup_graphics() {
    std::graphics::closegraph();
    std::cout << "graphics closed" << std::endl;
}

int main(int argc,char *argv[]) {
    int gd = DETECT, gm;
    std::graphics::initgraph(&gd, &gm, NULL);
    int gerr = std::graphics::graphresult();
    if (gerr != 0) {
        std::cout << "failed to open graphics mode, err: " <<  gerr << std::endl;
        return 1;
    }
    // graphics started
    std::atexit(cleanup_graphics);
    start_logo();
    return 0;
}
