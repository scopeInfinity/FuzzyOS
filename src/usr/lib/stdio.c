#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <sys/syscall.h>

int putchar(int c) {
    return SYSCALL_A2(SYSCALL_CONSOLE, SYSCALL_CONSOLE_SUB_PUTCHAR, c);
}

int puts(const char *s) {
    if(!s) return 0;
    int len = strlen(s);
    return SYSCALL_A3(SYSCALL_CONSOLE, SYSCALL_CONSOLE_SUB_PUTS_BUFFER, s, len);
}

static int vsnprintf(char *s, size_t n, const char *fmt, va_list args) {
    if(n==0) return 0;
    if(n==1) {
        s[0]='\0';
        return 0;
    }
    n--; // count non-null char

    size_t len = 0;
    int err = 0;
    for(;err >= 0 && len<n && (*fmt)!='\0' ; fmt++) {
        if((*fmt)!='%') {
            s[len++] = (*fmt);
            continue;
        }
        fmt++;
        if((*fmt)=='\0') break;

        // formatting options
        switch(*fmt) {
            char *innerstr;
            char sbuffer[68];
            case '%':
                s[len++]='%';
                break;
            case 'c':
                s[len++]=va_arg(args, char);
                break;
            case 's':
                innerstr = va_arg(args, const char*);
                while (len<n && (*innerstr)!='\0') {
                    s[len++]=*(innerstr++);
                }
                break;
            case 'd':
                itoa(va_arg(args, int), sbuffer, 10);
                innerstr = sbuffer;
                while (len<n && (*innerstr)!='\0') {
                    s[len++]=*(innerstr++);
                }
                break;
            case 'X':
            case 'x':  // not an priority for now :)
                itoa(va_arg(args, int), sbuffer, 16);
                innerstr = sbuffer;
                while (len<n && (*innerstr)!='\0') {
                    s[len++]=*(innerstr++);
                }
                break;
            default:
                err = -1;  // invalid format char, terminate
        }
    }
    s[len]='\0';
    if(err != 0) return err;
    return len;
}

int snprintf(char *s, size_t n, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(s, n, fmt, args);
    va_end(args);
    return ret;
}

int printf(const char *fmt, ...) {
    // limitation: it won't print more than 256 bytes in one go.
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    return puts(buffer);
}

char* gets(char *s) {
    char *og = s;
    while (1) {
        char c = getch();
        if (c == '\0') return NULL;
        putchar(c);
        if (c == '\n') {
            *s='\0';
            return og;
        }
        *(s++) = c;
    }
}

static int _all_file_handlers_buzy[LIMIT_MAX_FILE_OPEN]={0};
static FILE _all_file_handlers[LIMIT_MAX_FILE_OPEN];

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
    handler->cursor = 0;
    return handler;
}

int fclose(FILE *handler) {
    if (handler != NULL) {
        _all_file_handlers_buzy[handler->file_handler_id] = 0;
    }
}

char *fgets(char *buf, size_t n, FILE *file) {
    if(n == 0) {
        return NULL;
    }
    n--;  // number of char to read excluding null char.
    char *og_buf = buf;
    char _buffer[FILEIO_BUFFER_SIZE];
    while (n > 0) {
        // Assumes count <= FILEIO_BUFFER_SIZE
        int count = SYSCALL_A4(SYSCALL_FILE_OP, SYSCALL_FILE_SUB_READBUFFER, file->file_id, _buffer, file->cursor);
        if (count < 0) {
            // error
            return NULL;
        }
        if (count == 0) {
            // EOF
            break;
        }

        int max_iterations = min(n, count);
        int char_read_count = 0;
        int found_newline = 0;
        for (size_t i = 0; i < max_iterations; i++) {
            char_read_count++;
            if(_buffer[i]=='\n') {
                found_newline = 1;
                break;
            }
            *(buf++)=_buffer[i];
        }
        file->cursor += char_read_count;
        n-=char_read_count;
        if(found_newline) break;
    }
    *buf = '\0';
    return og_buf;
}

int fgetc(FILE *file) {

}
