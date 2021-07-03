#include <stdlib.h>

int atoi (const char *s) {
    int neg = 0;
    if ((*s) == '-' || (*s) == '+') {
        neg = ((*s) == '-');
        s++;
    }
    int num = 0;
    while((*s)!='\0') {
        num=num*10+(*s)-'0';
        s++;
    }
    return num;
}
