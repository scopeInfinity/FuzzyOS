#include <stdio.h>
#include <stddef.h>
#include <conio.h>
#include <lib/utils/output.h>
#include <sys/syscall.h>

int putchar(int c) {
    print_char(c);
    // TODO: Fix putchar return value.
    return 0;
}

int puts(const char *s) {
    print_line(s);
    // TODO: Fix puts return value.
    return 0;
}

char* gets(char *s) {
    while (1) {
        char c = getch();
        if (c == '\0') return NULL;
        if (c == '\n') return s;
        putchar(c);
        (*s) = c;
        s++;
    }
}

int _all_file_handlers_buzy[LIMIT_MAX_FILE_OPEN]={0};
FILE _all_file_handlers[LIMIT_MAX_FILE_OPEN];

FILE *fopen(char *filename, char *mode) {
    // TODO: mode is ignored for now.
    // And only read only mode is supported.
    int file_id = SYSCALL_A2(SYSCALL_FILE_OP, SYSCALL_FILE_SUB_OPEN, filename);
    if (file_id<0) return NULL;

    // find available file_handler
    int fh_id=0;
    while (fh_id<LIMIT_MAX_FILE_OPEN) {
        if(!_all_file_handlers_buzy[fh_id]) break;
        fh_id++;
    }
    if(fh_id==LIMIT_MAX_FILE_OPEN) {
        // no available file_handler found
        return NULL;
    }

    _all_file_handlers_buzy[fh_id] = 1;
    FILE *handler = &_all_file_handlers[fh_id];
    handler->file_handler_id = fh_id;
    handler->file_id = file_id;
    return handler;
}

int fclose(FILE *handler) {
    if (handler != NULL) {
        _all_file_handlers_buzy[handler->file_handler_id] = 0;
    }
}