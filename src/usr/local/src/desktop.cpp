// desktop environment
#include <conio.h>
#include <dirent.h>
#include <graphics.h>
#include <iostream.h>
#include <process.h>
#include <utility.h>
#include <vector.h>

const int WINDOW_HEIGHT = GRAPHICS_MAX_HEIGHT;
const int WINDOW_WIDTH = GRAPHICS_MAX_WIDTH;

enum IconType { DATA_FILE = 0, EXECUTABLE = 1 };

struct IconData {
    std::string fname;
    IconType type;
};

class GraphicsModeController {
    int gd, gm;
    bool is_graphics_enabled;

  public:
    GraphicsModeController() : is_graphics_enabled(false) {
        // assume to be in text-mode at start
    }

    void graphics_mode_enable() {
        if (is_graphics_enabled)
            return;
        gd = DETECT;

        std::graphics::initgraph(&gd, &gm, NULL);
        int gerr = std::graphics::graphresult();
        if (gerr != 0) {
            std::cout << "failed to open graphics mode, err: " << gerr
                      << std::endl;
            std::exit(1);
        }
        is_graphics_enabled = true;
    }

    void graphics_mode_disable() {
        if (!is_graphics_enabled)
            return;

        std::graphics::closegraph();
        is_graphics_enabled = false;
    }
} gmcontroller;

class Desktop {
    const int ICON_WIDTH = 160;
    const int ICON_COUNT_HOR = WINDOW_WIDTH / ICON_WIDTH;

    int width, height;
    std::vector<IconData> files;

    int text_height;
    int text_width;
    int cursor_id;

    int spawn(std::vector<std::string> &argv) {
        // argv[0] == executable filename
        const char **argvp = new const char *[argv.size() + 1];
        for (int i = 0; i < argv.size(); i++) {
            argvp[i] = argv[i].c_str();
        }
        argvp[argv.size()] = NULL;
        const char *filename = argvp[0];
        int pid = std::spawnv(filename, argvp);
        if (pid < 0) {
            // failed
            return pid;
        }
        int last_status_code;
        std::waitpid(pid, &last_status_code);
        delete[] argvp;
        return last_status_code;
    }

  public:
    Desktop(int width, int height) : width(width), height(height) {
        text_width = std::graphics::textwidth(" ");
        text_height = std::graphics::textheight(" ");

        cursor_id = 0;
        gmcontroller.graphics_mode_enable();
    }

    void refresh() {
        struct std::DIR dir;
        std::opendir(&dir);
        struct std::dirent *dp;

        files.clear();
        while ((dp = std::readdir(&dir)) != NULL) {
            bool executable = dp->flag & std::DIRENT_EXECUTABLE;
            IconData file{
                .fname = std::string(dp->d_name),
                .type = executable ? EXECUTABLE : DATA_FILE,
            };
            files.push_back(file);
        }
        draw();
    }

    void launch_app(int app_id) {
        // switch to text-mode
        gmcontroller.graphics_mode_disable();

        std::vector<std::string> argv;
        if (files[app_id].type == EXECUTABLE) {
            argv.push_back(files[app_id].fname);
        } else {
            // execute: more <filename>
            argv.push_back(std::string("more"));
            argv.push_back(files[app_id].fname);
        }

        std::clrscr();
        int rv = spawn(argv);

        std::cout << "exited: " << rv << std::endl;
        std::cout << "Press any key to return to desktop." << std::endl;
        std::getch();

        // // switch back to graphics mode
        gmcontroller.graphics_mode_enable();
    }

    void controller(char c) {
        // assumes at least one file.
        int id_count = files.size();

        switch (c) {
        case 'd':
            cursor_id = (cursor_id + 1) % id_count;
            break;
        case 'a':
            cursor_id = (cursor_id + id_count - 1) % id_count;
            break;
        case 'w':
            cursor_id = (cursor_id - ICON_COUNT_HOR + id_count) % id_count;
            break;
        case 's':
            cursor_id = (cursor_id + ICON_COUNT_HOR) % id_count;
            break;
        case '\n':
            // selected
            launch_app(cursor_id);
            break;
        }
    }

    void execute() {
        do {
            refresh();
            controller(std::getch());
        } while (1);
    }

    void draw_icon(int id, const IconData &file, int y_offset) {
        int x = (id % ICON_COUNT_HOR) * ICON_WIDTH;
        int y = (id / ICON_COUNT_HOR) * text_height + y_offset;

        const int half_border = 1;

        const int max_length = (ICON_WIDTH - half_border * 2) / text_width;
        std::string fname = file.fname;
        if (fname.length() > max_length) {
            fname[max_length] = '\0';
        }

        // highlight selected icon
        if (id == cursor_id) {
            std::graphics::setcolor(YELLOW);
        } else {
            std::graphics::setcolor(WHITE);
        }
        std::graphics::bar(x, y, x + ICON_WIDTH - 1, y + text_height - 1);

        // draw icon and text
        if (file.type == EXECUTABLE) {
            std::graphics::setcolor(GREEN);
        } else {
            // data file
            std::graphics::setcolor(BLACK);
        }
        std::graphics::outtextxy(x, y, fname.c_str());
    }

    void draw() {
        const int banner_border = 2;

        // draw banner
        {
            std::graphics::setcolor(LIGHT_BLUE);
            std::graphics::bar(0, 0, this->width - 1,
                               text_height + banner_border * 2 - 1);
            int x_offset = banner_border;
            std::graphics::setcolor(YELLOW);
            x_offset += std::graphics::outtextxy(x_offset, banner_border,
                                                 "| FuzzyOS | ");
            std::graphics::setcolor(BLACK);
            x_offset += std::graphics::outtextxy(x_offset, banner_border,
                                                 "Control: w,a,s,d,enter");
        }

        // draw icons
        {
            std::graphics::setcolor(LIGHT_GRAY);
            std::graphics::bar(0, text_height + banner_border * 2,
                               this->width - 1, this->height - 1);

            for (int i = 0; i < files.size(); i++) {
                draw_icon(i, files[i], text_height + banner_border * 2);
            }
        }
        std::graphics::graphflush();
    }
};

void start_dm() {
    std::graphics::graphautoflush_disable();
    std::graphics::setbkcolor(BLACK);

    Desktop d(GRAPHICS_MAX_WIDTH, GRAPHICS_MAX_HEIGHT);
    d.execute();
}

void cleanup_graphics() {
    gmcontroller.graphics_mode_disable();
    std::cout << "heap memory usage at exit "
              << (std::benchmark_get_heap_usage()) << " bytes" << std::endl;
    std::cout << "heap memory area at exit " << (std::benchmark_get_heap_area())
              << " bytes" << std::endl;
}

int show_usage() {
    std::cout << "desktop environment" << std::endl;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        // assumes user wants desktop --help
        return show_usage();
    }

    int gd = DETECT, gm;
    // graphics started
    std::atexit(cleanup_graphics);
    start_dm();
    return 0;
}
