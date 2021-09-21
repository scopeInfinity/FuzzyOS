#pragma once


namespace std {

// treat endl as character
extern const char endl;

class ostream {
public:
    ostream& operator<<(const char c);
    ostream& operator<<(const char* str);
    ostream& operator<<(const int num);
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