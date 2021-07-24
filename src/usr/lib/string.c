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

int strlen(char *str) {
    int n=0;
    while((*str)!='\0') {
        n++;
        str++;
    }
    return n;
}

char *strcpy(char *dest, const char *src) {
    void *og_dest = dest;
    while ((*src) != '\0') {
        *(dest++) = *(src++);
    }
    *(dest++) = '\0';
    return og_dest;
}

void *memcpy(void *dest, const void *src, unsigned int n) {
    void *og_dest = dest;
    while (n--) {
        *(char*)(dest++) = *(char*)(src++);
    }
    return og_dest;
}
