#ifndef LAB_05_POINT_LIST_H
#define LAB_05_POINT_LIST_H

#include "clist.h"

#include "stddef.h"

typedef struct {
    int x, y;
    intrusive_node_t node;
} point_t;

/**
 * Allocates memory for the point and inserts it
 * into the list.
 */
void add_point(intrusive_list_t *list, int x, int y);

/**
 * Removes all the entries of (x, y) from the list and deallocates memory.
 *
 * @return how many points were removed.
 */
size_t remove_point(intrusive_list_t *list, int x, int y);

/**
 * Basically, clears the list.
 */
void remove_all_points(intrusive_list_t *list);

#endif //LAB_05_POINT_LIST_H
