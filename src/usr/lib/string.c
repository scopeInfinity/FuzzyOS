#include <string.h>
#include <stddef.h>
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
    while ((*src) != NULL) {
        *(dest++) = *(src++);
    }
    *(dest++) = NULL;
    return og_dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    void *og_dest = dest;
    while ((*src) != NULL && n>0) {
        *(dest++) = *(src++);
        n--;
    }
    return og_dest;
}

void *memcpy(void *dest, const void *src, unsigned int n) {
    void *og_dest = dest;
    while (n--) {
        *(char*)(dest++) = *(char*)(src++);
    }
    return og_dest;
}

char *strchr(const char *str, char ch) {
    while ((*str) != NULL) {
        if ((*str) == ch) {
            return str;
        }
        str++;
    }
    return NULL;
}

char *strtok(char *str, const char *delim) {
    static char *last_it = NULL;
    static char last_char_stash;
    char *it;
    if (str == NULL) {
        it = last_it;
        *(it) = last_char_stash;
        if ((*it) == NULL) {
            // no more token left
            return NULL;
        }
        it++;
    } else {
        it = str;
        if ((*it) == NULL) {
            // empty string
            return NULL;
        }
    }

    char *end = it;
    while ((*end) != NULL && strchr(delim, *end) == NULL) {
        end++;
    }

    last_char_stash = *end;
    last_it = end;
    *end = NULL;
    return it;
}