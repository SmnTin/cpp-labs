#ifndef LAB_03
#define LAB_03

#include "stddef.h"

/*
 * Returns 0 on success.
 * Returns -1 if failed to allocate enough auxiliary memory.
 */
int mergesort(void *arr, size_t arr_len, size_t elem_size,
              int (*cmp)(const void *, const void *));

#endif //LAB_03
