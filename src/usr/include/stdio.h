#pragma once

#include <stddef.h>

// TODO: define stdio, stdout, stderr

int putchar(int c);
int puts(const char *s);
int printf(const char *strfmt, ...);

char* gets (char *s);

typedef struct FILE {
    // in user space.
    int file_handler_id;
    // in kernel/filesystem space
    int file_id;

    // 0 => start of file
    int cursor;
} FILE;

// TODO: Add API for reading list of files.
// Only readonly mode is supported.
#define SYSCALL_FILE_SUB_OPEN 0
#define SYSCALL_FILE_SUB_READBUFFER 1
#define SYSCALL_FILE_SUB_READ_DIR 2

// We are allocating FILE handler as global because we
// don't have heap memory manager yet.
#define LIMIT_MAX_FILE_OPEN 10
// same as sector size for better performance
#define FILEIO_BUFFER_SIZE 512

#define EOF (-1)

FILE *fopen(char *filename, char *mode);
int fgetc(FILE *file);
char *fgets(char *buf, size_t n, FILE *file);
int fclose(FILE *file);