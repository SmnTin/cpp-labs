#include "clist.h"

void init_list(intrusive_list_t *list) {
    list->head = NULL;
}

void add_node(intrusive_list_t *list, intrusive_node_t *node) {
    if (!node || !list)
        return;

    if (list->head)
        list->head->prev = node;
    node->next = list->head;
    node->prev = NULL;

    list->head = node;
}

void remove_node(intrusive_list_t *list, intrusive_node_t *node) {
    if (!list || !node)
        return;

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    if (list->head == node)
        list->head = node->next;
}

size_t get_length(const intrusive_list_t *list) {
    size_t length = 0u;
    if (!list)
        return length;

    intrusive_node_t *current = list->head;
    while (current) {
        ++length;
        current = current->next;
    }

    return length;
}
