#pragma once

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef uint32_t size_t;

#ifdef __cplusplus
}  // namespace std end
}  // extern C end
#endif
