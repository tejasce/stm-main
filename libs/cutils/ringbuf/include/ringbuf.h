#ifndef CUTILS_RINGBUF_H
#define CUTILS_RINGBUF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ringbuf rb_t; // ringbuf context handle

/*
 * @brief  Create a ring-buffer of fixed sized elements
 *
 * @param[in] base    Pointer to start of ring-buffer memory. If NULL then it
 *                    will allocate memory of size (entsz * entnum) for the
 *                    base buffer which will be freed when the rinbuf is
 *                    destroyed. If not NULL then the pointer is treated as
 *                    pre-allocated base buffer of same size
 * @param[in] entsz   Size of each entry in the ringbuf
 * @param[in] entnum  Number of entries in the ringbuf
 *
 * @return  Context handle for the ringbuf if success, NULL otherwise
 */
extern rb_t *rb_init(void *base, size_t entsz, size_t entnum);

/*
 * @brief  Destroy a previously created ring-buffer
 *
 * @param[in] rb  Context handle for a previously created ringbuf
 */
extern void rb_fini(rb_t *rb);

/*
 * @brief  Push an entry into the ring-buffer [thread-safe]
 *         Blocks indefinitely if ring-buffer is full
 *
 * @param[in] rb   Context handle for a previously created ringbuf
 * @param[in] src  Source entry buffer
 *                 Buf size is assumed to be of same size ringbuf was
 *                 initialized with
 *
 * @return  If success 0, errno otherwise
 *          EINVAL  Invalid argument(s) (if rb or src is NULL)
 */
extern int rb_push(rb_t *rb, const void *src);

/*
 * @brief  Pop an entry from the ring-buffer [thread-safe]
 *         Blocks indefinitely if ring-buffer is empty
 *
 * @param[in] rb   Context handle for a previously created ringbuf
 * @param[in] dst  Destination entry buffer to save popped entry into
 *                 Buf size is assumed to be of same size ringbuf was
 *                 initialized with
 *
 * @return  If success 0, errno otherwise
 *          EINVAL  Invalid argument(s) (if rb or dst is NULL)
 */
extern int rb_pop(rb_t *rb, void *dst);

/*
 * @brief  Push an entry into the ring-buffer [thread-safe]
 *         Returns immediately if ring-buffer is full
 *
 * @param[in] rb   Context handle for a previously created ringbuf
 * @param[in] src  Source entry buffer
 *                 Buf size is assumed to be of same size ringbuf was
 *                 initialized with
 *
 * @return  If success 0, errno otherwise
 *          EINVAL  Invalid argument(s) (if rb or src is NULL)
 *          EAGAIN  Ringbuf is full
 */
extern int rb_push_nb(rb_t *rb, const void *src);

/*
 * @brief  Pop an entry from the ring-buffer [thread-safe]
 *         Returns immediately if ring-buffer is empty
 *
 * @param[in] rb   Context handle for a previously created ringbuf
 * @param[in] dst  Destination entry buffer to save popped entry into
 *                 Buf size is assumed to be of same size ringbuf was
 *                 initialized with
 *
 * @return  If success 0, errno otherwise
 *          EINVAL  Invalid argument(s) (if rb or dst is NULL)
 *          EAGAIN  Ringbuf is empty
 */
extern int rb_pop_nb(rb_t *rb, void *dst);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_RINGBUF_H
