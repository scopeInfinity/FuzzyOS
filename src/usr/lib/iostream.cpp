#include <iostream.h>
#include <stdio.h>
#include <conio.h>

namespace std {
#define __IS_WHITESPACE(ch) ((ch)==' ' || (ch)=='\n' || (ch)=='\t')

const char endl = '\n';
ostream cout;
istream cin;


ostream& ostream::operator<<(const char c) { putchar(c); return *this; }
ostream& ostream::operator<<(const int num) { printf("%d", num); return *this; }
ostream& ostream::operator<<(const char *str) { puts(str); return *this; }
ostream& ostream::operator<<(const std::string &str) { puts(str.c_str()); return *this; }

istream& istream::operator>>(char &c) {
    do {
        this->get(c);
    } while (__IS_WHITESPACE(c));
    return *this;
}

istream& istream::operator>>(char *str) {
    char c;
    bool leading_whitespace = 1;
    while(1) {
        this->get(c);
        if (leading_whitespace) {
            if(__IS_WHITESPACE(c)) continue;
            leading_whitespace = 0;
        } else {
            // from second non-whitespace character
            if(__IS_WHITESPACE(c)) break;
        }
        *(str++) = c;
    };
    *(str++) = '\0';
    return *this;
}

istream& istream::operator>>(std::string &str) {
    str.clear();
    char c;
    bool leading_whitespace = 1;
    while(1) {
        this->get(c);
        if (leading_whitespace) {
            if(__IS_WHITESPACE(c)) continue;
            leading_whitespace = 0;
        } else {
            // from second non-whitespace character
            if(__IS_WHITESPACE(c)) break;
        }
        str += c;
    };
    return *this;
}

istream& istream::get(char &c) { c = getch(); putchar(c); return *this; }

istream& istream::get_line(char *str, std::size_t n, char delim) {
    char c;
    while (n-- > 0) {
        this->get(c);
        if (c == delim) break;
        *(str++) = c;
    }
    *(str) = '\0';
    return *this;
}

#undef __IS_WHITESPACE
}  // namespace std end
