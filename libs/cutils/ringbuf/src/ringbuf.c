#include <assert.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include <cutils/alloc.h>
#include "ringbuf.h"
#include "ringbuf_priv.h"

rb_t *
rb_init(void *base, size_t entsz, size_t entnum)
{
    assert(entnum > 1); // number of entries (or ringbuf depth) must be >1

    rb_t *rb = zmalloc(sizeof(rb_t));
    if (!base) {
        base = zmalloc(entsz * entnum);
        rb->flags |= RB_ALLOC;
    }
    rb->base = base;
    rb->entsz = entsz;
    rb->entnum = entnum;

    pthread_mutex_init(&rb->mut, NULL);
    pthread_cond_init(&rb->wr.cond, NULL);
    pthread_cond_init(&rb->rd.cond, NULL);

    return rb;
}

void
rb_fini(rb_t *rb)
{
    if (rb) {
        pthread_mutex_lock(&rb->mut);
        if (rb->base && (rb->flags & RB_ALLOC)) {
            free(rb->base);
        }
        pthread_mutex_unlock(&rb->mut);
        pthread_mutex_destroy(&rb->mut);
        pthread_cond_destroy(&rb->wr.cond);
        pthread_cond_destroy(&rb->rd.cond);
        free(rb);
    }
}

int
rb_push_worker(rb_t *rb, const void *src, rb_flag_t rb_flags)
{
    if (!rb || !src) {
        return EINVAL;
    }
    pthread_mutex_lock(&rb->mut);
    while (rb_full(rb)) {
        if (rb_flags & RB_NONBLOCK) {
            pthread_mutex_unlock(&rb->mut);
            return EAGAIN;
        }
        pthread_cond_wait(&rb->rd.cond, &rb->mut);
    }

    void *dst = rb_ent(rb, &rb->wr);
    memcpy(dst, src, rb->entsz); // NOLINT
    rb_end_step(rb, &rb->wr);
    pthread_cond_signal(&rb->wr.cond);
    pthread_mutex_unlock(&rb->mut);
    return 0;
}

int
rb_push(rb_t *rb, const void *src)
{
    return rb_push_worker(rb, src, 0);
}

int
rb_push_nb(rb_t *rb, const void *src)
{
    return rb_push_worker(rb, src, RB_NONBLOCK);
}

int
rb_pop_worker(rb_t *rb, void *dst, rb_flag_t rb_flags)
{
    if (!rb || !dst) {
        return EINVAL;
    }
    pthread_mutex_lock(&rb->mut);
    while (rb_empty(rb)) {
        if (rb_flags & RB_NONBLOCK) {
            pthread_mutex_unlock(&rb->mut);
            return EAGAIN;
        }
        pthread_cond_wait(&rb->wr.cond, &rb->mut);
    }

    void *src = rb_ent(rb, &rb->rd);
    memcpy(dst, src, rb->entsz); // NOLINT
    rb_end_step(rb, &rb->rd);
    pthread_cond_signal(&rb->rd.cond);
    pthread_mutex_unlock(&rb->mut);
    return 0;
}

int
rb_pop(rb_t *rb, void *dst)
{
    return rb_pop_worker(rb, dst, 0);
}

int
rb_pop_nb(rb_t *rb, void *dst)
{
    return rb_pop_worker(rb, dst, RB_NONBLOCK);
}
