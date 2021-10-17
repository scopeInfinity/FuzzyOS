#pragma once

#define SYSCALL_KEYBOARD 0
#define SYSCALL_PROCESS 1
#define SYSCALL_FILE_OP 2
#define SYSCALL_CONSOLE 3
#define SYSCALL_GRAPHICS 4

#define SYSCALL_A0(id) syscall((id), 0, 0, 0, 0)
#define SYSCALL_A1(id, a0) syscall((id), (a0), 0, 0, 0)
#define SYSCALL_A2(id, a0, a1) syscall((id), (a0), (a1), 0, 0)
#define SYSCALL_A3(id, a0, a1, a2) syscall((id), (a0), (a1), (a2), 0)
#define SYSCALL_A4(id, a0, a1, a2, a3) syscall((id), (a0), (a1), (a2), (a3))

extern int syscall(int id, int arg0, int arg1, int arg2, int arg3);
