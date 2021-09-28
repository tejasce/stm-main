#ifndef CUTILS_ALLOC_H
#define CUTILS_ALLOC_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief  Worker function that allocates memory of requested size.
 *         The allocated memory is filled with bytes of value zero.
 *
 * @param[in] count  Number of objects to allocate
 * @param[in] size   Size of each object
 * @param[in] nb     Flag for non-blocking call (if true returns immediately
 *                   when calloc() fails)
 *
 * @return  Pointer to allocated memory
 */
static inline void *
_zalloc(size_t count, size_t size, bool nb)
{
    void *ptr;
    while ((ptr = calloc(count, size)) == NULL) {
        if (nb) {
            break;
        } else {
            usleep(1000);
        }
    }
    return ptr;
}

/*
 * @brief  Wrapper function to try to allocate memory till it succeeds (blocking
 *         call). The allocated memory is filled with bytes of value zero.
 *
 * @param[in] count  Number of objects to allocate
 * @param[in] size   Size of each object
 *
 * @return  Pointer to allocated memory (or blocks indefinitely till _zcalloc()
 *          succeeds)
 */
static inline void *
zalloc(size_t count, size_t size)
{
    return _zalloc(count, size, false);
}

/*
 * @brief  Wrapper function to allocate memory (non-blocking call)
 *         The allocated memory is filled with bytes of value zero.
 *
 * @param[in] count  Number of objects to allocate
 * @param[in] size   Size of each object
 *
 * @return  Pointer to allocated memory, NULL if failed
 */
static inline void *
zalloc_nb(size_t count, size_t size)
{
    return _zalloc(count, size, true);
}

/*
 * @brief  Wrapper function to try to allocate memory till it succeeds (blocking
 *         call). The allocated memory is filled with bytes of value zero.
 *
 * @param[in] size   Size of memory to allocate
 *
 * @return  Pointer to allocated memory (or blocks indefinitely till _zcalloc()
 *          succeeds)
 */
static inline void *
zmalloc(size_t size)
{
    return _zalloc(1, size, false);
}

/*
 * @brief  Wrapper function to allocate memory (non-blocking call).
 *         The allocated memory is filled with bytes of value zero.
 *
 * @param[in] size   Size of memory to allocate
 *
 * @return  Pointer to allocated memory, NULL if failed
 */
static inline void *
zmalloc_nb(size_t size)
{
    return _zalloc(1, size, true);
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALLOC_H
