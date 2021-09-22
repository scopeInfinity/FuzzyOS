#include <iostream.h>
#include <stdio.h>
#include <conio.h>

namespace std {

const char endl = '\n';
ostream cout;
istream cin;

ostream& ostream::operator<<(const char c) { putchar(c); return *this; }
ostream& ostream::operator<<(const int num) { printf("%d", num); return *this; }
ostream& ostream::operator<<(const char *str) { puts(str); return *this; }
ostream& ostream::operator<<(const std::string &str) { puts(str.c_str()); return *this; }

istream& istream::operator>>(char &c) { c = getch(); putchar(c); return *this; }
istream& istream::operator>>(char *str) { gets(str); return *this; }

}  // namespace std end
