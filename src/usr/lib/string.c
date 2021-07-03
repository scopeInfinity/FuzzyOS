#include <string.h>
#include <ctype.h>

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
