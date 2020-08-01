#include <lib/utils/input.h>
#include <lib/utils/string.h>

static char buffer_num[20];

extern char getch();

void read_line(char *str) {
    int i = 0;
    while(1) {
        str[i]=getch();
        if(str[i]=='\r') {
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