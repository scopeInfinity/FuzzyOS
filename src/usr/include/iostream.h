#pragma once

#include <string.h>

namespace std {

// treat endl as character
extern const char endl;

class ostream {
  public:
    ostream &operator<<(const char c);
    ostream &operator<<(const int num);
    ostream &operator<<(const double num);
    ostream &operator<<(const std::string &str);
    ostream &operator<<(const char *str);
};

class istream {
  public:
    istream &operator>>(char &c);
    istream &operator>>(char *str);
    istream &operator>>(std::string &str);
    istream &operator>>(int &x);

    istream &get(char &c);
    istream &get_line(char *str, std::size_t n, char delim = '\n');
};

extern ostream cout;
extern istream cin;

std::istream &get_line(std::istream &i, std::string &str);

} // namespace std