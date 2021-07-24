#pragma once

#include <stddef.h>

int putchar(int c);
int puts(const char *s);

char* gets (char *s);

typedef struct FILE {
    // in user space.
    int file_handler_id;
    // in kernel/filesystem space
    int file_id;
} FILE;

// TODO: Add API for reading list of files.
// Only readonly mode is supported.
#define SYSCALL_FILE_SUB_OPEN 0

// We are allocating FILE handler as global because we
// don't have heap memory manager yet.
#define LIMIT_MAX_FILE_OPEN 10

FILE *fopen(char *filename, char *mode);
int fclose(FILE *file);