// logo program
#include <iostream.h>
#include <utility.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
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

std::vector<std::string> split(std::string &str) {
    std::vector<std::string> rv;
    std::size_t len = str.length();
    char del = ' ';
    std::size_t last = 0;
    while(last<len) {
        std::size_t next = str.find(del, last);
        if(next == std::string::npos) {
            next = len;
        }
        std::size_t copy_size = next-last;
        if(copy_size>0) {
            rv.push_back(str.substr(last, copy_size));
        }
        last = next+1;
    }
    return rv;
}

class Display {
    int height;
    typedef std::pair<int,int> Point;  // x, y
    #define _x first
    #define _y second
    std::vector<std::pair<Point, Point> > lines;
    // logo
    Point home;
    Point current;
    double angle;  // in radians
    bool pen_active;  // true implies draw when pen moves
    bool turtle_visiblity;
public:
    Display(int height) : height(height),
            current(WINDOW_WIDTH / 2, height / 2),
            angle(M_PI / 2),
            pen_active(true),
            turtle_visiblity(true) {
        home = current;
    }

    static Point at_distance(Point src, double dis, double angle) {
        return Point(
            src._x + dis*std::cos(angle),
            src._y - dis*std::sin(angle)
        );
    }

    int get_x() { return current._x; }
    int get_y() { return current._y; }
    double get_angle() { return angle; }

    void action_forward(int len) {
        Point next = current;
        next._x += std::round(len*std::cos(angle));
        next._y -= std::round(len*std::sin(angle));  // graphics and maths y coordinates are flipped
        action_move(next);
    }

    void action_move(Point dst) {
        if (pen_active) {
            lines.push_back(std::make_pair(current, dst));
        }
        current = dst;
    }

    void action_rotate(int degree) {
        angle = std::fmod(angle + degree*M_PI/180, 2*M_PI);
    }

    void action_rotate_abs(int degree) {
        angle = std::fmod(degree*M_PI/180, 2*M_PI);
    }

    void action_pen_active(bool is_active) {
        pen_active = is_active;
    }

    void action_turtle_visibility(bool visible) {
        turtle_visiblity = visible;
    }

    void action_clear() {
        lines.clear();
    }

    int command_handler(std::string &cmd, std::string &input, std::size_t &start) {
        if (cmd == "fd" || cmd == "forward") {
            std::string arg=parse_next_token(input, start);
            if (arg.empty()) return 2;
            int len = std::atoi(arg.c_str());
            action_forward(len);
            return 0;
        }
        if (cmd == "bk" || cmd == "back") {
            std::string arg=parse_next_token(input, start);
            if (arg.empty()) return 2;
            int len = std::atoi(arg.c_str());
            action_forward(-len);
            return 0;
        }
        if (cmd == "lt" || cmd == "left") {
            std::string arg=parse_next_token(input, start);
            if (arg.empty()) return 2;
            int angle_deg = std::atoi(arg.c_str());
            action_rotate(angle_deg);
            return 0;
        }
        if (cmd == "rt" || cmd == "right") {
            std::string arg=parse_next_token(input, start);
            if (arg.empty()) return 2;
            int angle_deg = std::atoi(arg.c_str());
            action_rotate(-angle_deg);
            return 0;
        }
        if (cmd == "pu" || cmd == "penup") {
            action_pen_active(false);
            return 0;
        }
        if (cmd == "pd" || cmd == "pendown") {
            action_pen_active(true);
            return 0;
        }
        if (cmd == "home") {
            action_move(home);
            action_rotate_abs(90);
            return 0;
        }
        if (cmd == "ht" || cmd == "hideturtle") {
            action_turtle_visibility(false);
            return 0;
        }
        if (cmd == "st" || cmd == "showturtle") {
            action_turtle_visibility(true);
            return 0;
        }
        if (cmd == "ct" || cmd == "clear") {
            action_clear();
            return 0;
        }
        if (cmd == "exit") {
            std::exit(0);
        }
        return 0;
    }

    std::string parse_next_token(std::string &input, std::size_t &start) {
        while(start<input.length()) {
            std::size_t next = input.find(' ', start);
            if(next == std::string::npos) {
                next = input.length();
            }
            std::size_t copy_size = next-start;
            std::size_t last = start;
            start = next+1;
            if(copy_size>0) {
                return input.substr(last, copy_size);
            }
        }
        return "";  // no more token
    }

    int parse(std::string input) {
        std::size_t start = 0;
        while (1) {
            std::string cmd = parse_next_token(input, start);
            if (cmd.empty()) break;
            int err = command_handler(cmd, input, start);
            if (err) {
                return err;
            }
        }
        return 0;
    }

    void draw() {
        std::graphics::setcolor(WHITE);
        std::graphics::bar(0, 0, WINDOW_WIDTH-1, this->height-1);
        std::graphics::setcolor(BLACK);

        for(const auto &line: lines) {
            std::graphics::line(line.first.first, line.first.second,
                                line.second.first, line.second.second);
        }

        if (turtle_visiblity) {
            std::graphics::setcolor(GREEN);
            // draw turtle
            //  - turtle image is > at angle = 0
            double turtle_len = 10;

            Point e1 = at_distance(current, turtle_len, angle + M_PI*5/6);
            Point e2 = at_distance(current, turtle_len, angle + M_PI*7/6);
            Point eb = at_distance(current, turtle_len/2, angle + M_PI);

            std::graphics::line(e1._x, e1._y, current._x, current._y);
            std::graphics::line(e1._x, e1._y, eb._x, eb._y);
            std::graphics::line(e2._x, e2._y, current._x, current._y);
            std::graphics::line(e2._x, e2._y, eb._x, eb._y);
        }

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

    void update_log(Display &display) {
        int current_x = display.get_x();
        int current_y = display.get_y();
        int angle_deg = std::round(display.get_angle()*180/M_PI);

        int index = 2; // current location
        auto &log = data[index];
        log = "loc: ";
        char buffer[16];
        std::itoa(current_x, buffer, 10);
        log += buffer;
        log += ',';
        std::itoa(current_y, buffer, 10);
        log += buffer;

        std::itoa(angle_deg, buffer, 10);
        log += "  dir: ";
        log += buffer;
        log += " deg";
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
    Console console;
    Display display;
public:
    Interpreter() : display(console.get_y_offset()) {
    }
    void execute() {
        while(1) {
            console.update_log(display);
            display.draw();
            console.draw();
            std::string input = console.read();
            int err = display.parse(input);
            console.set_err(err);
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
