#pragma once

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

typedef struct va_list {
    int *base;  // using int* to jmp at 4 bytes gap
} va_list;

#define va_start(list, last) ((list).base = (((int*)&(last))+1))
#define va_end(list) (list).base = NULL
#define va_arg(list, type) (*(type*)((list).base++))
#define va_copy(dst, src) (dst).base = (src).base

#ifdef __cplusplus
}  // namespace std end
}  // extern C end
#endif
