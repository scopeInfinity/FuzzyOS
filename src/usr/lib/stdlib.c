#include <stdlib.h>
#include <process.h>
#include <sys/syscall.h>

int min(int a, int b) {
    return (a<b)?a:b;
}

int max(int a, int b) {
    return (a>b)?a:b;
}

int abs(int a) {
    return (a>=0)?a:-a;
}

int atoi(const char *s) {
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

void itoa(int num, char *s, int base) {
    // TODO: add assert on non-positive base.
    if(num==0) {
        *(s++)='0';
        *(s++)='\0';
        return;
    }
    if (base == 10 && num<0) {
        *(s++) = '-';
        num=-num;
    }

    int digit_count = 0;
    int bnum = num;
    while(bnum) {
        digit_count++;
        bnum/=base;
    }

    *(s+digit_count) = '\0';
    while((digit_count--)>0) {
        int c = num%base;
        num /= base;
        *(s+digit_count) = (c<10)?'0'+c:'A'-10+c;
    }
}

void exit(int status) {
    SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_EXIT, status);
    // process should be marked as ready to kill, wait util it's killed.
    // process yield should be enough complete kill.
    yield();
    while(1);
}
