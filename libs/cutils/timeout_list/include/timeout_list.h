#ifndef CUTILS_TIMEOUT_LIST_H
#define CUTILS_TIMEOUT_LIST_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct timeout_list_ctx tolist_ctx_t; // timeout_list context handle

/*
 * @brief Initialize a [thread-safe] timeout-list
 *        A timeout-list timestamps the entries inserted in it and guarantees
 *        to return entries no older than user specified timeout value. Older
 *        entries are automatically deallocated and deleted from the list.
 *
 * @param[in] timeout_ms  Timeout value in milliseconds
 * @param[in] entsz       Size of each entry in the list
 *
 * @return  Context handle for the timeout-list if success, NULL otherwise
 */
extern tolist_ctx_t *timout_list_init(uint64_t timeout_ms, size_t entsz);

/*
 * @brief Destroy a previously created timeout-list
 *
 * @param[in] ctx  Context handle for previously created timeout-list
 */
extern void timeout_list_fini(tolist_ctx_t *ctx);

/*
 * @brief  Insert an entry in timeout-list
 *         The entry must be of size same as the list was initialized with.
 *         Memory is allocated for the entry by the list.
 *
 * @param[in] ctx  Context handle for previously created timeout-list
 * @param[in] ent  Data entry to insert in the list
 *
 * @return  If success 0, negative errno otherwise
 *          -ENOMEM  Failed to allocate memory for ent
 */
extern int timout_list_put(tolist_ctx_t *ctx, void *ent);

/*
 * @brief  Retrieve entries from the timeout-list (newest entries first)
 *         Retrieved entries aren't immediately deleted from the list
 *         but only when they are expired which is determined at the
 *         time of this API call.
 *
 * @param[in] ctx    Context handle for previously created timeout-list
 * @param[in] buf    Input buffer to store retrieved entries in
 * @param[in] bufsz  Size of input buffer (must be >= entsz at list init)
 *
 * @return  Number of bytes written in input buffer if success, negative errno
 *          otherwise
 *          -EINVAL  Insufficient bufsz
 */
extern int timout_list_get(tolist_ctx_t *ctx, void *buf, size_t bufsz);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_TIMEOUT_LIST_H
