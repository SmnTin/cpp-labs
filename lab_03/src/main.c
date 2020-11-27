#include "mergesort.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/*
 * Returns 0 on success.
 * Returns -1 on failure.
 */
static int read_int(const char *str, void *ptr) {
    // https://stackoverflow.com/questions/2024648/convert-a-string-to-int-but-only-if-really-is-an-int
    char *end;
    *(int *) ptr = (int) strtol(str, &end, 10);
    return (*end == '\0') ? 0 : -1;
}

static int read_char(const char *str, void *ptr) {
    if (strlen(str) != 1)
        return -1;

    *(char *) ptr = str[0];
    return 0;
}

static int read_str(const char *str, void *ptr) {
    *(const char **) ptr = (const char *) str;
    return 0;
}

/*
 * Type-safe max and min with protection from double evaluation.
 * https://stackoverflow.com/questions/3437404/min-and-max-in-c
 * It uses GNU statement expression extension
 * which is not defined by any C standard.
 * Being completely strict max and min functions should be
 * defined for each integer type separately which is not
 * really convenient and would pollute this short example code.
 */
#define max(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
   ({ __typeof__ (a) _ma = (a); \
       __typeof__ (b) _mb = (b); \
     _ma < _mb ? _ma : _mb; })

#define clamp(val, min_, max_) \
   max(min_, min(max_, val))

/*
 * Returns 0 on success.
 * Returns -1 on failure.
 */
static int print_int(const void *ptr) {
    int err = printf("%d", *(const int *) ptr);
    return clamp(err, -1, 0);
}

static int print_char(const void *ptr) {
    int err = printf("%c", *(const char *) ptr);
    return clamp(err, -1, 0);
}

static int print_str(const void *ptr) {
    int err = printf("%s", *(const char *const *) ptr);
    return clamp(err, -1, 0);
}

static int int_lt_cmp(const void *a, const void *b) {
    return *(const int *) a - *(const int *) b;
}

static int char_lt_cmp(const void *a, const void *b) {
    return *(const char *) a - *(const char *) b;
}

static int str_lt_cmp(const void *a, const void *b) {
    return strcmp(*(const char *const *) a, *(const char *const *) b);
}

/*
 * Detects type out of [type_str] and sets other pointers accordingly.
 *
 * Returns 0 on success.
 * Returns -1 if the given type is unknown.
 */
static int detect_type(const char *type_str, size_t *type_size,
                       int (**read_elem)(const char *, void *),
                       int (**print_elem)(const void *),
                       int (**lt_cmp)(const void *, const void *)) {
    enum {
        N = 3
    };

    static char *type_names[N] = {"int", "char", "str"};
    static size_t type_sizes[N] = {sizeof(int), sizeof(char), sizeof(char *)};

    static int (*lt_cmps[N])(const void *, const void *) = {
            &int_lt_cmp, &char_lt_cmp, &str_lt_cmp
    };
    static int (*readers[N])(const char *, void *) = {
            &read_int, &read_char, &read_str
    };
    static int (*printers[N])(const void *) = {
            &print_int, &print_char, &print_str
    };

    for (int i = 0; i < N; ++i) {
        if (strcmp(type_str, type_names[i]) == 0) {
            *type_size = type_sizes[i];
            *lt_cmp = lt_cmps[i];
            *print_elem = printers[i];
            *read_elem = readers[i];
            return 0;
        }
    }
    return -1;
}

/*
 * Parses array from string representations of elements.
 * Returns 0 on success.
 * Returns -1 if the reading failed.
 */
static int parse_array(void *arr, const char *const * elems_strs,
                       size_t arr_len, size_t elem_size,
                       int (*read_elem)(const char *, void *)) {
    for (size_t i = 0; i < arr_len; ++i)
        if ((*read_elem)(elems_strs[i], (char *) arr + elem_size * i) < 0)
            return -1;

    return 0;
}

/*
 * Parses arguments. The first argument is expected to be string
 * representation of the type. Other arguments are string 
 * representations of the array elements.
 *
 * [dest] is allocated automatically. It must be manually freed 
 * after use.
 *
 * Returns 0 on success.
 * Returns -1 if the reading failed.
 * Returns -2 if the memory allocation failed.
 */

static int parse_args(int argc, const char *const * argv, void **arr,
                      size_t *arr_len, size_t *type_size,
                      int (**read_elem)(const char *, void *),
                      int (**print_elem)(const void *),
                      int (**lt_cmp)(const void *, const void *)) {
    *arr = NULL;

    if (argc <= 1)
        return -1;

    // The first arg is skipped because it carries the program name
    if (detect_type(argv[1], type_size, read_elem, print_elem, lt_cmp) < 0)
        return -1;

    *arr_len = argc - 2;

    // Allocating at least 1 element because allocating 0 bytes is UB
    *arr = malloc(max(*arr_len, (size_t) 1) * (*type_size));
    if (arr == NULL)
        return -2;

    return parse_array(*arr, argv + 2, *arr_len, *type_size, *read_elem);
}

/*
 * Prints array from the anonymous array pointer.
 *
 * Returns 0 on success.
 * Return -1 on failure.
 */
static int print_array(const void *arr, size_t arr_len,
                       size_t elem_size, int (*print_elem)(const void *)) {
    for (size_t i = 0; i < arr_len; ++i) {
        if ((*print_elem)((const char *) arr + i * elem_size) < 0)
            return -1;
        printf(" ");
    }
    printf("\n");

    return 0;
}

int main(int argc, char **argv) {
    size_t arr_len, elem_size;
    int (*read_elem)(const char *, void *);
    int (*print_elem)(const void *);
    int (*lt_cmp)(const void *, const void *);
    void *arr;

    int err = parse_args(argc, (const char *const *) argv, &arr,
                         &arr_len, &elem_size, &read_elem,
                         &print_elem, &lt_cmp);

    if (err == -2)
        fprintf(stderr, "Memory allocation while array reading failed.");
    else if (err == -1)
        fprintf(stderr, "Array reading failed.");
    else if (mergesort(arr, arr_len, elem_size, lt_cmp) == -1)
        fprintf(stderr, "Memory allocation while sorting failed.");
    else if (print_array(arr, arr_len, elem_size, print_elem) == -1)
        fprintf(stderr, "Array printing failed.");

    if (arr)
        free(arr);

    return 0;
}
