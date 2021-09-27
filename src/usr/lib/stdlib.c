#include <stdlib.h>
#include <stddef.h>
#include <process.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <math.h>

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
    if(neg) num = -num;
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

void ftoa(double num, char *s) {
    // not a good implementation
    if (isnan(num)) {
        *(s++) = 'n';
        *(s++) = 'a';
        *(s++) = 'n';
        *(s++) = '\0';
        return;
    }
    if (num < 0) {
        *(s++) = '-';
        num = - num;
    }
    if(num==0) {
        *(s++)='0';
        *(s++)='\0';
        return;
    }
    int exp = 0;
    while(num>=1) {
        num /= 10;
        exp++;
    }
    while(num<0.1) {
        num *= 10;
        exp--;
    }

    *(s++)='0';
    *(s++)='.';

    for(int i=0;i<8;i++) {
        num *= 10;
        int msb = num;
        *(s++)='0'+msb;
        num -= msb;
        if (num < 1e-16) {
            break;
        }
    }

    if(exp!=0) {
        *(s++) = 'e';
        if(exp>0) *(s++) = '+';
        itoa(exp, s, 10);
    } else {
        *(s++) = '\0';
    }
}

static void (*_at_exit_handler)(void) = NULL;
void atexit(void (*handler)(void)) {
    // supports only one handler
    _at_exit_handler = handler;
}

static void at_exit_trigger() {
    // make sure to kill the program even if exit_handler crashes
    static int at_exit_triggered = 0;
    if (at_exit_triggered == 0) {
        // first call of this program
        at_exit_triggered = 1;
        if(_at_exit_handler) {
            _at_exit_handler();
        }
    } else {
        // call sequence
        // exit -> at_exit_trigger -> _at_exit_handler -> exit (possibly due to crash)
        // do nothing
    }
}

void exit(int status) {
    at_exit_trigger();
    SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_EXIT, status);
    // process should be marked as ready to kill, wait util it's killed.
    // process yield should be enough complete kill.
    yield();
    while(1);
}

/*
non-optimal malloc
*/
extern void* get_current_esp();  // defined in stdlib.asm
extern char _heap_start[];  // defined in linker.ld
static int heap_head_offset = 0;
static const int heap_stack_safety_gap = 1024; // keep 1 kb free between stack and heap

void* malloc(size_t size) {
    void* loc = _heap_start + heap_head_offset;
    void* max_loc = get_current_esp()-heap_stack_safety_gap-size;
    if(loc>max_loc) {
        printf("failed to allocate more memory\n");
        exit(-1);
        return NULL;
    }
    heap_head_offset += size;
    return loc;
}

void free(void* ptr) {
    if(ptr == NULL) return;
    // current version of malloc is non-optimal and doesn't
    // do any free operation.
}
