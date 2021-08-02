#include<stddef.h>

extern void syscall_strncpy_user_to_kernel(int user_ds, char *src_es_address, char *dest_ds_address, size_t size);
extern void syscall_strncpy_kernel_to_user(int user_ds, char *dest_address, char *src_address, size_t size);
