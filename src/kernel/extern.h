#include<stddef.h>

extern void syscall_strccpy_es_to_ds(char *src_es_address, char *dest_ds_address, size_t size);

extern void syscall_context_switching_fix_es();