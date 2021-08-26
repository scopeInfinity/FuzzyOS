#pragma once

#include <stddef.h>

int strcmp(char *l, char *r);
int strcmpi(char *l, char *r);
int strlen(char *str);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
void *memcpy(void *dest, const void *src, unsigned int n);

char *strchr(const char *str, char ch);
char *strtok(char *str, const char *delim);
