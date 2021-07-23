#pragma once

#include <stddef.h>

int putchar(int c);
int puts(const char *s);

char* gets (char *s);

typedef struct FILE {
} FILE;

// TODO: Add API for reading list of files.
// Only readonly mode is supported.
#define SYSCALL_FILE_SUB_OPEN 0

FILE *fopen(char *filename, char *mode);
int fclose(FILE *file);