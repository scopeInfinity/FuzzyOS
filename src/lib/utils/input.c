#include <stdlib.h>

#include <lib/utils/input.h>
#include <lib/utils/output.h>
#include <sys/syscall.h>
static char buffer_num[20];

extern int getch_low();

char getch_in() { return SYSCALL_A0(SYSCALL_KEYBOARD); }

// TODO(scopeinfinity): Fix local variables with syscall(maybe?)
int i = 0;
void read_line(char *str) {
    i = 0;
    while (1) {
        // Bug: Wierd hack to mitigate another hack.
        // Using following instead of str[i]=getch_in();
        char z = getch_in();
        str[i] = z;

        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';
            print_char('\n');
            break;
        }
        print_char(str[i]);
        i++;
    }
}

int read_int() {
    read_line(buffer_num);
    return atoi(buffer_num);
}