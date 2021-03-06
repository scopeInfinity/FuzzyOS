#include <lib/syscall/syscall.h>
#include <lib/utils/input.h>
#include <lib/utils/output.h>
#include <lib/utils/string.h>

static char buffer_num[20];

extern int getch_low();

char getch() {
    return SYSCALL_A0(SYSCALL_KEYBOARD);
}

void read_line(char *str) {
    int i = 0;
    while(1) {
        // Bug: Wierd hack to mitigate another hack.
        // Using following instead of str[i]=getch();
        char z = getch();
        str[i] = z;

        if(str[i]=='\r' || str[i]=='\n') {
            str[i]='\0';
            print_char('\n');
            break;
        }
        print_char(str[i]);
        i++;
    }
}

int read_int() {
    read_line(buffer_num);
    return parse_int(buffer_num);
}