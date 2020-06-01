#ifndef __LIB_UTIL_STRING
#define __LIB_UTIL_STRING

#include <lib/syscalls/io.h>

char buffer_num[20];

int parse_int(char *s) {
    int x = 0;
    for(int i=0;s[i]!='\0';i++) {
        x=x*10+s[i]-'0';
    }
    return x;
}

int read_int() {
    read_line(buffer_num);
    return parse_int(buffer_num);
}

int strcmp(char *l, char *r) {
    int i=0,j=0;
    while(l[i]!='\0' && r[j]!='\0') {
        if(l[i]!=r[j]) {
            return l[i]-r[j];
        }
        i++;j++;
    }
    if(l[i]=='\0' && r[j]=='\0') {
        return 0;
    }
    if(l[i]=='\0') {
        return 1;
    }
    return -1;
}

#endif