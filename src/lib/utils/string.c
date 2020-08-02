#include <lib/utils/string.h>

int parse_int(char *s) {
    int x = 0;
    for(int i=0;s[i]!='\0';i++) {
        x=x*10+s[i]-'0';
    }
    return x;
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

char tolower(char x) {
    if(x>='A' && x<='Z') {
        return x-'A'+'a';
    }
    return x;
}

int strcmpi(char *l, char *r) {
    int i=0,j=0;
    while(l[i]!='\0' && r[j]!='\0') {
        char a = tolower(l[i]);
        char b = tolower(r[j]);
        if(a!=b) {
            return a-b;
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
