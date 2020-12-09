#include "clist.h"

#include "point_list.h"

#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"

/**
 * Reads points from file and inserts them into the list.
 * Note that the list is not cleared before call so actually
 * the data is appended.
 * Assumes that given file is correct.
 *
 * @return number of arguments taken on success
 * 0 when the command name doesn't match
 * -1 on failure
 */
static int handle_loadtext(intrusive_list_t *list, int argc, char **argv) {
    if (argc < 2)
        return 0;
    if (strcmp("loadtext", argv[0]) != 0)
        return 0;

    FILE *input = fopen((const char *) argv[1], "r");
    if (!input) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return -1;
    }

    int x, y;
    while (fscanf(input, "%d %d", &x, &y) == 2) {
        point_t *point = malloc(sizeof(point_t));
        assert(point);

        point->x = x, point->y = y;

        add_node(list, &point->node);
    }

    fclose(input);
    return 2;
}

static void savetext_each(intrusive_node_t *node, void *data) {
    FILE *output = (FILE *) data;
    point_t *point = container_of(node, point_t, node);
    fprintf(output, "%d %d\n", point->x, point->y);
}

/**
 * Writes points to a text file.
 *
 * @return number of arguments taken on success
 * 0 when the command name doesn't match
 * -1 on failure
 */
static int handle_savetext(intrusive_list_t *list, int argc, char **argv) {
    if (argc < 2)
        return 0;
    if (strcmp("savetext", argv[0]) != 0)
        return 0;

    FILE *output = fopen((const char *) argv[1], "w");
    apply(list, &savetext_each, output);
    fclose(output);

    return 2;
}

static inline bool is_little_endian(void) {
    uint16_t val = 0x0001;
    return *(uint8_t *) &val;
}

static inline int reverse_bytes(int v) {
    return (v >> 24) |
           ((v & 0xFF0000) >> 8) |
           ((v & 0xFF00) << 8) |
           ((v & 0xFF) << 24);
}

// Even though this code has conditional jump,
// it is well optimized by the compiler.
// The code was studied with the godbolt.com
static inline int little_endian_to_int(int v) {
    if (is_little_endian())
        return v;
    return reverse_bytes(v);
}

static inline int int_to_little_endian(int v) {
    return little_endian_to_int(v);
}

// To convert 3-byte negative to 5-byte negative
// all we have to do is just to fill 4-th byte with ones.
static inline int conv_3byte_to_4byte(int b3) {
    if (!(b3 & 0xA00000))
        return b3;
    int b4 = (b3 | (0xFF << 24));
    return b4;
}

/**
 * Reads points from a binary file and inserts them into the list.
 *
 * @return number of arguments taken on success
 * 0 when the command name doesn't match
 * -1 on failure
 */
static int handle_loadbin(intrusive_list_t *list, int argc, char **argv) {
    if (argc < 2)
        return 0;
    if (strcmp("loadbin", argv[0]) != 0)
        return 0;

    FILE *input = fopen((const char *) argv[1], "rb");
    if (!input) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return -1;
    }

    // X and Y in big-endian
    int lx = 0, ly = 0;
    while (fread((uint8_t *) &lx, 1, 3, input) == 3 &&
           fread((uint8_t *) &ly, 1, 3, input) == 3) {
        int x = conv_3byte_to_4byte(little_endian_to_int(lx));
        int y = conv_3byte_to_4byte(little_endian_to_int(ly));

        point_t *point = malloc(sizeof(point_t));
        point->x = x, point->y = y;

        add_node(list, &point->node);
    }

    fclose(input);

    return 2;
}

static void savebin_each(intrusive_node_t *node, void *data) {
    FILE *output = (FILE *) data;
    point_t *point = container_of(node, point_t, node);

    int lx = int_to_little_endian(point->x); // X in little-endian
    int ly = int_to_little_endian(point->y); // Y in little-endian

    fwrite((uint8_t *) &lx, 1, 3, output);
    fwrite((uint8_t *) &ly, 1, 3, output);
}

/**
 * Writes points to a binary file.
 *
 * @return number of arguments taken on success
 * 0 when the command name doesn't match
 * -1 on failure
 */
static int handle_savebin(intrusive_list_t *list, int argc, char **argv) {
    if (argc < 2)
        return 0;
    if (strcmp("savebin", argv[0]) != 0)
        return 0;

    FILE *output = fopen((const char *) argv[1], "wb");
    apply(list, &savebin_each, output);
    fclose(output);

    return 2;
}

static void print_point(intrusive_node_t *node, void *data) {
    const char *format = (const char *) data;
    point_t *point = container_of(node, point_t, node);
    printf(format, point->x, point->y);
}

/**
 * Prints contents of the list.
 * Assumes that given format is correct.
 *
 * @return number of arguments taken on success
 * 0 when the command name doesn't match
 * -1 on failure
 */
static int handle_print(intrusive_list_t *list, int argc, char **argv) {
    if (argc < 2)
        return 0;
    if (strcmp("print", argv[0]) != 0)
        return 0;

    apply(list, &print_point, argv[1]);
    printf("\n");

    return 2;
}

static void count_nodes(intrusive_node_t *node, void *data) {
    int *cnt = (int *) data;
    ++(*cnt);
}

/**
 * Print length of the list command.
 * @return number of arguments taken on success
 * 0 when the command name doesn't match
 * -1 on failure
 */
static int handle_count(intrusive_list_t *list, int argc, char **argv) {
    if (argc < 1)
        return 0;
    if (strcmp("count", argv[0]) != 0)
        return 0;

    int count = 0;
    apply(list, &count_nodes, &count);
    printf("%d\n", (int) count);

    return 1;
}

/**
 * @param argc number of arguments passed from command line
 * @param argv arguments passed from command line
 */
static void handle_cmd(int argc, char **argv, intrusive_list_t *list) {
    // The first argument contains program name
    --argc;
    ++argv;

    enum {
        N = 6
    };

    static int (*cmd_handlers[N])(intrusive_list_t *, int, char **) = {
            &handle_print, &handle_count,
            &handle_loadtext, &handle_savetext,
            &handle_loadbin, &handle_savebin
    };

    while (argc > 0) {
        int res = 0;

        for (size_t i = 0; i < N; ++i) {
            res = (*cmd_handlers[i])(list, argc, argv);
            if (res > 0) {
                argc -= res;
                argv += res;
            }

            // Either failure or success
            if (res != 0)
                break;
        }

        if (res == 0)
            fprintf(stderr, "Unknown action.\n");
        if (res <= 0)
            break;
    }
}

int main(int argc, char **argv) {
    intrusive_list_t list;
    init_list(&list);

    handle_cmd(argc, argv, &list);

    remove_all_points(&list);

    return 0;
}
