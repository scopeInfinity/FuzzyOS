#pragma once

void interrupt_register_0x32_syscall();

int interrupt_handler_0x32_syscall_handler(int id, int arg0, int arg1, int arg2, int arg3, int user_ds);
