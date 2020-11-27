#include "mergesort.h"

#include "stdlib.h"
#include "string.h"

static void mergesort_impl(char *arr, char *aux, size_t arr_len, size_t elem_size,
                           int (*cmp)(const void *, const void *)) {
    if (arr_len <= 1)
        return;

    size_t m = arr_len / 2, r = arr_len;
    mergesort_impl(arr, aux, m, elem_size, cmp);
    mergesort_impl(arr + m * elem_size, aux, r - m, elem_size, cmp);

    size_t p1 = 0, p2 = m, q = 0;
    while (p1 < m || p2 < r) {
        char *dest = aux + (q++) * elem_size;
        char *elem1 = arr + p1 * elem_size;
        char *elem2 = arr + p2 * elem_size;

        if (p2 == r || (p1 < m && (*cmp)(elem1, elem2) < 0))
            memmove(dest, elem1, elem_size), ++p1;
        else
            memmove(dest, elem2, elem_size), ++p2;
    }
    for (size_t i = 0; i < arr_len; ++i)
        memmove(arr + i * elem_size, aux + i * elem_size, elem_size);
}

int mergesort(void *array, size_t elements, size_t element_size,
              int (*comparator)(const void *, const void *)) {
    void *aux = malloc(elements * element_size);
    if (!aux)
        return -1;

    mergesort_impl(array, aux, elements, element_size, comparator);

    free(aux);
    return 0;
}
