#pragma once

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

#define SYSCALL_KEYBOARD_SUB_GETCH 0
#define SYSCALL_KEYBOARD_SUB_KBHIT 1

int getch();
int kbhit();

void clrscr();

#ifdef __cplusplus
} // namespace std end
} // extern C end
#endif
