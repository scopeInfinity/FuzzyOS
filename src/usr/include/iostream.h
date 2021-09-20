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

extern ostream cout;

}  // namespace std end