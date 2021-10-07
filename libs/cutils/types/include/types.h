#ifndef CUTILS_TYPES_H
#define CUTILS_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PTR_TO_ADDR(p) ((uintptr_t)(p))
#define ADDR_TO_PTR(a) ((void *)(uintptr_t)(a))

#define BIT(n) (1 << (n))

#ifdef __cplusplus
}
#endif

#endif // CUTILS_TYPES_H
