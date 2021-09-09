#pragma once

#define SYSCALL_KEYBOARD_SUB_GETCH       0
#define SYSCALL_KEYBOARD_SUB_KBHIT       1

int getch();
int kbhit();

void clrscr();