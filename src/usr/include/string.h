#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

int strcmp(const char *l, const char *r);
int strcmpi(const char *l, const char *r);
int strlen(const char *str);
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


#ifdef __cplusplus
#include <vector.h>

namespace std {

template<typename CharT>
class basic_string {
    std::vector<CharT> _data;

public:
    basic_string();
    basic_string(std::size_t n, CharT c);
    basic_string(const CharT* str);
    basic_string(const CharT* str, std::size_t n);

    CharT& back();
    CharT& front();
    CharT& at(std::size_t pos);
    CharT& operator[](std::size_t pos);
    const CharT *c_str() const;
};

using string = basic_string<char>;

}  // namespace std end

#include <string.tcc>

#endif
