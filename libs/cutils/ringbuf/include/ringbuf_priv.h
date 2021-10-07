#ifndef CUTILS_RINGBUF_PRIV_H
#define CUTILS_RINGBUF_PRIV_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#include <cutils/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// ringbuf flags (for internal use only)
enum {
    RB_ALLOC = BIT(0), // base buffer is allocated in rb_init()
    RB_NONBLOCK = BIT(1), // don't block if ringbuf is temporarily unavailable
};
typedef uint8_t rb_flag_t;

// ringbuf end definition
typedef struct {
    uint16_t idx;
    pthread_cond_t cond;
} rb_end_t;

// ringbuf context definition
typedef struct ringbuf {
    void *base;
    size_t entsz;
    size_t entnum;
    rb_flag_t flags;
    pthread_mutex_t mut;
    rb_end_t wr;
    rb_end_t rd;
} rb_t;

/*
 * @brief  Check if ringbuf is empty
 *
 * @param[in] rb  Context handle for a previously created ringbuf
 *
 * @return  True ringbuf is empty, false otherwise
 */
static inline bool
rb_empty(rb_t *rb)
{
    return rb->wr.idx == rb->rd.idx;
}

/*
 * @brief  Check if ringbuf is full
 *
 * @param[in] rb  Context handle for a previously created ringbuf
 *
 * @return  True ringbuf is full, false otherwise
 */
static inline bool
rb_full(rb_t *rb)
{
    return (rb->wr.idx + 1) % rb->entnum == rb->rd.idx;
}

/*
 * @brief  Step ringbuf index by 1
 *
 * @param[in] rb  Context handle for a previously created ringbuf
 * @param[in] e   Pointer to ringbuf read or write end
 */
static inline void
rb_end_step(rb_t *rb, rb_end_t *e)
{
    e->idx = (e->idx + 1) % rb->entnum;
}

/*
 * @brief  Fetch pointer reference to a ringbuf entry
 *
 * @param[in] rb  Context handle for a previously created ringbuf
 * @param[in] e   Pointer to ringbuf read or write end
 *
 * @return  Pointer reference to the ringbuf entry
 */
static inline void *
rb_ent(rb_t *rb, rb_end_t *e)
{
    return rb->base + e->idx * rb->entsz;
}

/*
 * @brief  Worker function to push an entry into the ring-buffer [thread-safe]
 *
 * @param[in] rb        Context handle for a previously created ringbuf
 * @param[in] src       Source entry buffer
 * @param[in] rb_flags  ringbuf flags
 *
 * @return  If success 0, errno otherwise
 *          EINVAL  Invalid argument(s) (if rb or src is NULL)
 *          EAGAIN  Ringbuf is full
 */
static int rb_push_worker(rb_t *rb, const void *src, rb_flag_t rb_flags);

/*
 * @brief  Worker function to pop an entry from the ring-buffer [thread-safe]
 *
 * @param[in] rb        Context handle for a previously created ringbuf
 * @param[in] dst       Destination entry buffer to save popped entry into
 * @param[in] rb_flags  ringbuf flags
 *
 * @return  If success 0, errno otherwise
 *          EINVAL  Invalid argument(s) (if rb or dst is NULL)
 *          EAGAIN  Ringbuf is empty
 */
static int rb_pop_worker(rb_t *rb, void *dst, rb_flag_t rb_flags);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_RINGBUF_PRIV_H
