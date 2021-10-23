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
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *dest, const unsigned char c, size_t n);

char *strchr(const char *str, char ch);
char *strtok(char *str, const char *delim);

#ifdef __cplusplus
} // namespace std end
} // extern C end
#endif

#ifdef __cplusplus
#include <vector.h>

namespace std {

template <typename CharT> class basic_string {
    std::vector<CharT> _data;

  public:
    const static std::size_t npos = -1;

    typedef CharT *iterator;
    typedef const CharT *const_iterator;

    basic_string();
    basic_string(std::size_t n, CharT c);
    basic_string(const CharT *str);
    basic_string(const CharT *str, std::size_t n);
    basic_string(const basic_string<CharT> &o);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void clear();
    void pop_back();
    CharT &back();
    CharT &front();
    CharT &at(std::size_t pos);
    CharT &operator[](std::size_t pos);
    const CharT *c_str() const;
    bool empty() const;
    std::size_t length() const;

    std::size_t find(char c, std::size_t pos = 0) const;
    basic_string<CharT> substr(int start, std::size_t len = npos);

    basic_string<CharT> &operator=(const basic_string<CharT> &o);
    basic_string<CharT> &operator+=(const CharT c);
    basic_string<CharT> &operator+=(const CharT *o);
    basic_string<CharT> &operator+=(const basic_string<CharT> &o);
};

using string = basic_string<char>;

} // namespace std

template <typename CharT>
inline bool operator==(const std::basic_string<CharT> &a, const CharT *b) {
    return std::strcmp(a.c_str(), b) == 0;
}
template <typename CharT>
inline bool operator==(const CharT *a, const std::basic_string<CharT> &b) {
    return std::strcmp(a, b.c_str()) == 0;
}
template <typename CharT>
inline bool operator==(const std::basic_string<CharT> &a,
                       const std::basic_string<CharT> &b) {
    return std::strcmp(a.c_str(), b.c_str()) == 0;
}

template <typename CharT>
inline bool operator!=(const std::basic_string<CharT> &a, const CharT *b) {
    return !operator==(a, b);
}
template <typename CharT>
inline bool operator!=(const CharT *a, const std::basic_string<CharT> &b) {
    return !operator==(a, b);
}
template <typename CharT>
inline bool operator!=(const std::basic_string<CharT> &a,
                       const std::basic_string<CharT> &b) {
    return !operator==(a, b);
}

template <typename CharT>
inline bool operator<(const std::basic_string<CharT> &a, const CharT *b) {
    return std::strcmp(a.c_str(), b) < 0;
}
template <typename CharT>
inline bool operator<(const CharT *a, const std::basic_string<CharT> &b) {
    return std::strcmp(a, b.c_str()) < 0;
}
template <typename CharT>
inline bool operator<(const std::basic_string<CharT> &a,
                      const std::basic_string<CharT> &b) {
    return std::strcmp(a.c_str(), b.c_str()) < 0;
}

template <typename CharT>
inline bool operator>(const std::basic_string<CharT> &a, const CharT *b) {
    return std::strcmp(a.c_str(), b) > 0;
}
template <typename CharT>
inline bool operator>(const CharT *a, const std::basic_string<CharT> &b) {
    return std::strcmp(a, b.c_str()) > 0;
}
template <typename CharT>
inline bool operator>(const std::basic_string<CharT> &a,
                      const std::basic_string<CharT> &b) {
    return std::strcmp(a.c_str(), b.c_str()) > 0;
}

template <typename CharT>
inline bool operator<=(const std::basic_string<CharT> &a, const CharT *b) {
    return !operator>(a, b);
}
template <typename CharT>
inline bool operator<=(const CharT *a, const std::basic_string<CharT> &b) {
    return !operator>(a, b);
}
template <typename CharT>
inline bool operator<=(const std::basic_string<CharT> &a,
                       const std::basic_string<CharT> &b) {
    return !operator>(a, b);
}

template <typename CharT>
inline bool operator>=(const std::basic_string<CharT> &a, const CharT *b) {
    return !operator<(a, b);
}
template <typename CharT>
inline bool operator>=(const CharT *a, const std::basic_string<CharT> &b) {
    return !operator<(a, b);
}
template <typename CharT>
inline bool operator>=(const std::basic_string<CharT> &a,
                       const std::basic_string<CharT> &b) {
    return !operator<(a, b);
}

#include <string.tcc>

#endif
