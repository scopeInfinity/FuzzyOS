#pragma once

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

int atoi (const char *s);

// only base 2, 8, 10 and 16 are supported
// for others the result can be undefined.
// And only base 10 is considered as signed int.
void itoa(int num, char *s, int base);
int min(int, int);
int max(int, int);
int abs(int a);

void exit(int status);

#ifdef __cplusplus
}  // namespace std end
}  // extern C end
#endif
