#pragma once

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

typedef struct va_list {
    void *base;
} va_list;

#define va_start(list, last) ((list).base = ((void *)(&(last) + 1)))
#define va_end(list) (list).base = NULL
#define va_arg(list, type)                                                     \
    (*(type *)((list).base += sizeof(type), \ 
                                     (list)                                    \
                                                    .base -                    \
                                                sizeof(type)))
#define va_copy(dst, src) (dst).base = (src).base

#ifdef __cplusplus
} // namespace std end
} // extern C end
#endif
