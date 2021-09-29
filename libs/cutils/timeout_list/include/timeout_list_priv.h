#ifndef CUTILS_TIMEOUT_LIST_PRIV_H
#define CUTILS_TIMEOUT_LIST_PRIV_H

#include <pthread.h>
#include <cutils/list.h>
#include <cutils/timeout_list.h>

#ifdef __cplusplus
extern "C" {
#endif

// timeout-list context definition
typedef struct timeout_list_ctx {
    uint64_t timeout_ms;
    size_t entsz;
    list_t l;
    pthread_mutex_t mut;
} tolist_ctx_t;

// list node containg a timout-list entry
typedef struct {
    void *ent;
    uint64_t ts_ms;
    list_node_t node;
} tolist_ent_t;

/*
 * @brief  Allocate memory for an entry node in a timeout-list
 *
 * @param[in] ctx  Context handle for previously created timeout-list
 * @param[in] ent  Entry data
 *
 * @return  Pointer to allocated entry if success, NULL otherwise
 */
extern tolist_ent_t *alloc_tolist_ent(tolist_ctx_t *ctx, void *ent);

/*
 * @brief  Free memory of previously allocated timeout-list node
 *
 * @param[in] ent  Previously allocated entry node
 */
extern void free_tolist_ent(tolist_ent_t *ent);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_TIMEOUT_LIST_PRIV_H
