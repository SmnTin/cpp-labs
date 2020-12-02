#ifndef CLIST_H_
#define CLIST_H_

#include "stddef.h"

typedef struct intrusive_node {
    struct intrusive_node *prev;
    struct intrusive_node *next;
} intrusive_node_t;

typedef struct {
    intrusive_node_t *head;
} intrusive_list_t;

#define container_of(ptr, type, member) (type *) ((char *) (ptr) - offsetof(type, member))

void init_list(intrusive_list_t *list);

void add_node(intrusive_list_t *list, intrusive_node_t *node);

void remove_node(intrusive_list_t *list, intrusive_node_t *node);

size_t get_length(const intrusive_list_t *list);

#endif
