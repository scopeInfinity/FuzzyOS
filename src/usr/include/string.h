#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

int strcmp(char *l, char *r);
int strcmpi(char *l, char *r);
int strlen(char *str);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
void *memcpy(void *dest, const void *src, unsigned int n);
void *memset(void *dest, const unsigned char c, size_t n);

char *strchr(const char *str, char ch);
char *strtok(char *str, const char *delim);

#ifdef __cplusplus
}  // namespace std end
}  // extern C end
#endif
