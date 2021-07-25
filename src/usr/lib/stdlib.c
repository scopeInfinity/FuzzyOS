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

void itoa (int num, char *s) {
    // base-10 for now.
    if(num==0) {
        *(s++)='0';
        *(s++)='\0';
        return;
    }
    if (num<0) {
        *(s++) = '-';
        num=-num;
    }

    int num_og = num;
    while (num) {
        num/=10;
        s++;
    }
    *(s)='\0';
    num=num_og;
    while (num) {
        *(--s)='0'+num%10;
        num/=10;
    }
}
