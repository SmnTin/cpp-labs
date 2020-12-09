#ifndef CLIST_H_
#define CLIST_H_

#include "stddef.h"

typedef struct intrusive_node {
    struct intrusive_node *prev;
    struct intrusive_node *next;
} intrusive_node_t;

typedef struct {
    intrusive_node_t head;
} intrusive_list_t;

#define container_of(ptr, type, member) (type *) ((char *) (ptr) - offsetof(type, member))

static inline intrusive_node_t *first_node(intrusive_list_t *list) {
    return list->head.next;
}

static inline intrusive_node_t *last_node(intrusive_list_t *list) {
    return list->head.prev;
}

void init_list(intrusive_list_t *list);

void add_node(intrusive_list_t *list, intrusive_node_t *node);

void remove_node(intrusive_list_t *list, intrusive_node_t *node);

/**
 * Applies operation on every point in the list.
 * @param data : additional data that is passed to op on every call
 */
void apply(intrusive_list_t *list, void (*op)(intrusive_node_t *node, void *data), void *data);

#endif
