#include <iostream.h>
#include <stdio.h>

namespace std {

const char endl = '\n';
ostream cout;

ostream& ostream::operator<<(const char c) { putchar(c); return *this; }
ostream& ostream::operator<<(const char *str) { puts(str); return *this; }
ostream& ostream::operator<<(const int num) { printf("%d", num); return *this; }

}  // namespace std end
