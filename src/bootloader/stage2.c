#define    C_BLACK               0x0
#define    C_BLUE                0x1
#define    C_GREEN               0x2
#define    C_CYAN                0x3
#define    C_RED                 0x4
#define    C_MAGENTA             0x5
#define    C_BROWN               0x6
#define    C_LIGHT_GRAY          0x7
#define    C_DARK_GRAY           0x8
#define    C_LIGHT_BLUE          0x9
#define    C_LIGHT_GREEN         0xA
#define    C_LIGHT_CYAN          0xB
#define    C_LIGHT_RED           0xC
#define    C_LIGHT_MAGENTA       0xD
#define    C_YELLOW              0xE
#define    C_WHITE               0xF

#define INT_MAX                  0xFFFFFFFF

#define WINDOW_HEIGHT            0x18
#define WINDOW_WIDTH             0x4F
#define CLOCKS_PER_MS            56230 // 5% of Host CPU

#define SLEEP_FACTOR_NUM         3     // Possiblity of improvement.
#define SLEEP_FACTOR_DENO        2
#define SLEEP_BURST_MS           INT_MAX/CLOCKS_PER_MS/SLEEP_FACTOR_NUM*SLEEP_FACTOR_DENO

#define make_color(fg, bg) ((unsigned char)((fg) + ((bg)<<4)))

extern void _low_print(char str[], unsigned short n,
                       unsigned char x,unsigned char y,
                       unsigned char color);

extern void _low_put_chars(char c,unsigned short count, unsigned char color);
extern void _low_move_xy(unsigned char x, unsigned char y, unsigned char page);
extern void _low_clear_screen(unsigned char color,
                              unsigned char x1,unsigned char y1,
                              unsigned char x2, unsigned char y2);
extern void _low_sleep(unsigned int half_instructions_count);

void run();

char message_welcome[] = "C says 'Hello World'";

unsigned char CURRENT_X = 0;
unsigned char CURRENT_Y = 0;

void move_xy(unsigned char x, unsigned char y) {
    CURRENT_X=x;
    CURRENT_Y=y;
    _low_move_xy(x,y,0);
}

void move_xy_diff(unsigned char dx, unsigned char dy) {
    CURRENT_X+=dx;
    CURRENT_Y+=dy;
    _low_move_xy(CURRENT_X,CURRENT_Y,0);
}

void put_char(char c, unsigned char color) {
    _low_put_chars(c,1, color);
    move_xy_diff(1, 0);
}

void put_chars(char c,unsigned short count, unsigned char color) {
    _low_put_chars(c,count, color);
    move_xy_diff(count, 0);
}

void put_string(char *str, unsigned char color) {
    while((*str)!='\0') {
        put_char(*str, color);
        str++;
    }
}

void sleep(unsigned int ms) {
    while(ms>0) {
        unsigned int fms = ms>SLEEP_BURST_MS?SLEEP_BURST_MS:ms;
        unsigned int cycles = CLOCKS_PER_MS*fms/SLEEP_FACTOR_DENO*SLEEP_FACTOR_NUM;
        _low_sleep(cycles);
        ms-=fms;
    }
}

void entry_stage() {
    move_xy(6, 11);
    put_string(message_welcome, make_color(C_GREEN, C_BLACK));
    sleep(1000);
    run();
    while(1);
}

char application_list[5][10] = {"Sample 1", "Sample 2", "Sample 3", "Sample 4", "Sample 5"};
void print_applications(unsigned char x, unsigned char y, char *list, unsigned char count, unsigned char max_strlen) {
    for(int i=0;i<count;i++) {
        move_xy(x,y+i);
        put_string("X ",make_color(C_BLUE, C_LIGHT_GRAY));
        put_string(list,make_color(C_BLACK, C_LIGHT_GRAY));
        list+=max_strlen;
    }
}

void run() {
    _low_clear_screen(make_color(C_WHITE, C_DARK_GRAY), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    _low_clear_screen(make_color(C_WHITE, C_LIGHT_GRAY), 1, 1, WINDOW_WIDTH-1, WINDOW_HEIGHT-1);
    move_xy(1,0);
    put_string("Fuzzy OS", make_color(C_WHITE, C_DARK_GRAY));
    move_xy(3,3);
    put_string("Applications:", make_color(C_BLACK, C_LIGHT_GRAY));
    print_applications(3,4, (char*)application_list, 5, 10);
}