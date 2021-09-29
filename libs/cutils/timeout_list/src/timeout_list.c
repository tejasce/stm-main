#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <cutils/alloc.h>
#include <cutils/list.h>
#include <cutils/time.h>
#include <cutils/types.h>
#include "timeout_list.h"
#include "timeout_list_priv.h"

tolist_ctx_t *
timout_list_init(uint64_t timeout_ms, size_t entsz)
{
    tolist_ctx_t *ctx = zmalloc(sizeof(tolist_ctx_t));
    ctx->timeout_ms = timeout_ms;
    ctx->entsz = entsz;
    list_init(&ctx->l, offsetof(tolist_ent_t, node));
    pthread_mutex_init(&ctx->mut, NULL);
    return ctx;
}

void
timeout_list_fini(tolist_ctx_t *ctx)
{
    if (ctx) {
        pthread_mutex_lock(&ctx->mut);
        tolist_ent_t *e = NULL;
        while ((e = list_delete_tail(&ctx->l))) {
            free_tolist_ent(e);
        }
        list_fini(&ctx->l);
        pthread_mutex_unlock(&ctx->mut);
        pthread_mutex_destroy(&ctx->mut);
        free(ctx);
    }
}

tolist_ent_t *
alloc_tolist_ent(tolist_ctx_t *ctx, void *ent)
{
    tolist_ent_t *e = zmalloc(sizeof(tolist_ent_t));
    e->ent = zmalloc(ctx->entsz);
    memcpy(e->ent, ent, ctx->entsz); // NOLINT
    e->ts_ms = gettsc_ms();
    return e;
}

void
free_tolist_ent(tolist_ent_t *ent)
{
    if (ent) {
        if (ent->ent) {
            free(ent->ent);
        }
        free(ent);
    }
}

int
timout_list_put(tolist_ctx_t *ctx, void *ent)
{
    assert(ctx && ent);
    int err = 0;
    pthread_mutex_lock(&ctx->mut);
    tolist_ent_t *e = alloc_tolist_ent(ctx, ent);
    if (!e) {
        err = -ENOMEM;
    } else {
        list_insert_head(&ctx->l, e);
    }
    pthread_mutex_unlock(&ctx->mut);
    return err;
}

int
timout_list_get(tolist_ctx_t *ctx, void *buf, size_t bufsz)
{
    assert(ctx && buf);
    if (bufsz < ctx->entsz) {
        return -EINVAL;
    }

    int off = 0;
    pthread_mutex_lock(&ctx->mut);
    if (!list_empty(&ctx->l)) {
        tolist_ent_t *e = NULL;
        uint64_t ts_ms = get_timestamp_ms(-ctx->timeout_ms);

        // first discard entries older than timeout
        while ((e = list_tail(&ctx->l)) && e->ts_ms <= ts_ms) {
            list_delete(&ctx->l, e);
            free_tolist_ent(e);
        }

        // remaining entries meet the "freshness" criterion
        for (e = list_head(&ctx->l); e && off < bufsz;
             e = list_next(&ctx->l, e)) {
            memcpy(buf + off, e->ent, MIN(bufsz - off, ctx->entsz)); // NOLINT
            off += ctx->entsz;
        }
    }
    pthread_mutex_unlock(&ctx->mut);
    return off;
}
