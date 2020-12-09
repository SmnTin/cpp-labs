#include "clist.h"

void init_list(intrusive_list_t *list) {
    list->head.next = &list->head;
    list->head.prev = &list->head;
}

void add_node(intrusive_list_t *list, intrusive_node_t *node) {
    if (!node || !list)
        return;

    intrusive_node_t *last = list->head.prev;
    list->head.prev = node;
    last->next = node;
    node->next = &list->head;
    node->prev = last;
}

void remove_node(intrusive_list_t *list, intrusive_node_t *node) {
    if (!list || !node)
        return;

    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void apply(intrusive_list_t *list, void (*op)(intrusive_node_t *node, void *data), void *data) {
    intrusive_node_t *current = first_node(list);
    while (current != &list->head) {
        (*op)(current, data);
        current = current->next;
    }
}
