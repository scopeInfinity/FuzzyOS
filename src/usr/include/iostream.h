#pragma once

#include <string.h>

namespace std {

// treat endl as character
extern const char endl;

class ostream {
public:
    ostream& operator<<(const char c);
    ostream& operator<<(const int num);
    ostream& operator<<(const std::string &str);
    ostream& operator<<(const char* str);
};

class istream {
public:
    istream& operator>>(char &c);
    istream& operator>>(char *str);
    istream& operator>>(int &num);
};

extern ostream cout;
extern istream cin;

}  // namespace std end