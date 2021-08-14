#pragma once

// return kernel_file_id if found.
// if successful (non-negative return value), entry is updated.
int file_handler_find(char *filename, union FFSFileEntry *entry);

int syscall_2_file_handler(int operation, int a1, int a2, int a3);
