#include "point_list.h"

#include "stdlib.h"
#include "assert.h"

void add_point(intrusive_list_t *list, int x, int y) {
    point_t *point = malloc(sizeof(point_t));
    assert(point);
    point->x = x, point->y = y;

    add_node(list, &point->node);
}

size_t remove_point(intrusive_list_t *list, int x, int y) {
    size_t removed_cnt = 0u;
    intrusive_node_t *current = first_node(list);

    while (current != &list->head) {
        point_t *point = container_of(current, point_t, node);
        intrusive_node_t *next = current->next;

        if (point->x == x && point->y == y) {
            ++removed_cnt;
            remove_node(list, current);
            free(point);
        }
        current = next;
    }

    return removed_cnt;
}

void remove_all_points(intrusive_list_t *list) {
    intrusive_node_t *current = first_node(list);
    while (current != &list->head) {
        point_t *point = container_of(current, point_t, node);
        intrusive_node_t *next = current->next;

        remove_node(list, current);
        free(point);

        current = next;
    }
}
