/*
 * Copyright 2017 Dell Inc. or its subsidiaries.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CUTILS_LIST_H
#define CUTILS_LIST_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Inline doubly-linked list routines
 *
 * list_init(l, off)            initialize a list; nodes at offset 'off'
 * list_fini(l)                 finish using a list; must be empty
 * list_prev(l, n)              return node before n, NULL if none
 * list_next(l, n)              return node after n, NULL if none
 * list_head(l)                 return list head, NULL if empty
 * list_tail(l)                 return list tail, NULL if empty
 * list_insert_before(l, b, n)  insert node n before b
 * list_insert_after(l, a, n)   insert node n after a
 * list_insert_head(l, n)       insert node n at head of the list
 * list_insert_tail(l, n)       insert node n at tail of the list
 * list_delete(l, n)            delete node n from the list
 * list_delete_head(l)          delete head of the list and return it, or NULL
 * list_delete_tail(l)          delete tail of the list and return it, or NULL
 * list_empty(l)                return true if the list is empty, false if not
 * list_move(src, dst)          move src to dst and clear src
 * list_node_swap(l, n1, n2)    swap nodes n1 and n2, both must be non-NULL
 */

struct list_node;

typedef struct list_node {
    struct list_node *n_prev;
    struct list_node *n_next;
} list_node_t;

typedef struct {
    size_t l_off;
    list_node_t l_anchor;
} list_t;

static inline void *__attribute__((always_inline)) __attribute__((pure))
list_node_to_data(const list_t *l, const list_node_t *n)
{
    if (n == &l->l_anchor) {
        return NULL;
    }
    return (void *)((uintptr_t)n - l->l_off);
}

static inline list_node_t *__attribute__((always_inline)) __attribute__((pure))
list_node_from_data(const list_t *l, const void *data)
{
    if (data == NULL) {
        return (list_node_t *)&l->l_anchor;
    }
    return (list_node_t *)((uintptr_t)data + l->l_off);
}

static inline void __attribute__((always_inline))
list_node_insert(list_node_t *t, list_node_t *p, list_node_t *n)
{
    p->n_next = t;
    t->n_prev = p;
    t->n_next = n;
    n->n_prev = t;
}

static inline void __attribute__((always_inline))
list_node_delete(list_node_t *t)
{
    list_node_t *p = t->n_prev;
    list_node_t *n = t->n_next;

    p->n_next = n;
    n->n_prev = p;
    t->n_next = NULL;
    t->n_prev = NULL;
}

static inline void
list_init(list_t *l, size_t off)
{
    l->l_off = off;
    list_node_insert(&l->l_anchor, &l->l_anchor, &l->l_anchor);
}

static inline void
list_fini(list_t *l)
{
    assert(l->l_anchor.n_next == &l->l_anchor);
    assert(l->l_anchor.n_prev == &l->l_anchor);
    list_node_delete(&l->l_anchor);
}

static inline void *__attribute__((always_inline))
list_prev(list_t *l, void *target)
{
    return list_node_to_data(l, list_node_from_data(l, target)->n_prev);
}

static inline void *__attribute__((always_inline))
list_next(list_t *l, void *target)
{
    return list_node_to_data(l, list_node_from_data(l, target)->n_next);
}

static inline void *__attribute__((always_inline)) list_head(list_t *l)
{
    return list_next(l, NULL);
}

static inline void *__attribute__((always_inline)) list_tail(list_t *l)
{
    return list_prev(l, NULL);
}

static inline void __attribute__((always_inline))
list_insert_before(list_t *l, void *before, void *target)
{
    list_node_t *t = list_node_from_data(l, target);
    list_node_t *n = list_node_from_data(l, before);
    list_node_t *p = n->n_prev;

    list_node_insert(t, p, n);
}

static inline void __attribute__((always_inline))
list_insert_after(list_t *l, void *after, void *target)
{
    list_node_t *t = list_node_from_data(l, target);
    list_node_t *p = list_node_from_data(l, after);
    list_node_t *n = p->n_next;

    list_node_insert(t, p, n);
}

static inline void __attribute__((always_inline))
list_insert_head(list_t *l, void *target)
{
    list_insert_after(l, NULL, target);
}

static inline void __attribute__((always_inline))
list_insert_tail(list_t *l, void *target)
{
    list_insert_before(l, NULL, target);
}

static inline void __attribute__((always_inline))
list_delete(list_t *l, void *target)
{
    list_node_delete(list_node_from_data(l, target));
}

static inline void *__attribute__((always_inline)) list_delete_head(list_t *l)
{
    void *head = list_head(l);

    if (head) {
        list_delete(l, head);
    }
    return head;
}

static inline void *__attribute__((always_inline)) list_delete_tail(list_t *l)
{
    void *tail = list_tail(l);

    if (tail) {
        list_delete(l, tail);
    }
    return tail;
}

static inline bool __attribute__((always_inline)) list_empty(list_t *l)
{
    return list_head(l) == NULL;
}

static inline void
list_move(list_t *src, list_t *dst)
{
    list_init(dst, src->l_off);
    if (src->l_anchor.n_prev != &src->l_anchor) {
        list_node_insert(&dst->l_anchor, src->l_anchor.n_prev,
                         src->l_anchor.n_next);
        list_node_insert(&src->l_anchor, &src->l_anchor, &src->l_anchor);
    }
}

static inline void
list_node_swap(list_t *l, void *t1, void *t2)
{
    assert(t1 && t2);

    if (t1 != t2) {
        void *t1_prev = list_prev(l, t1);
        void *t2_prev = list_prev(l, t2);
        if (t1 == t2_prev) {
            list_delete(l, t1);
            list_insert_after(l, t2, t1);
        } else if (t2 == t1_prev) {
            list_delete(l, t2);
            list_insert_after(l, t1, t2);
        } else {
            list_delete(l, t1);
            list_delete(l, t2);
            list_insert_after(l, t2_prev, t1);
            list_insert_after(l, t1_prev, t2);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_LIST_H
