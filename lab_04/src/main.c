#include "clist.h"

#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

typedef struct {
    int x, y;
    intrusive_node_t node;
} point_t;

/**
 * Allocates memory for the point and inserts it
 * into the list.
 */
static void add_point(intrusive_list_t *list, int x, int y) {
    point_t *point = malloc(sizeof(point_t));
    assert(point);
    point->x = x, point->y = y;

    add_node(list, &point->node);
}

/**
 * Removes all the entries of (x, y) from the list and deallocates memory.
 * @return how many points were removed.
 */
static size_t remove_point(intrusive_list_t *list, int x, int y) {
    size_t removed_cnt = 0u;
    intrusive_node_t *current = list->head;

    while (current) {
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

/**
 * Prints the contents of list into standard output.
 */
static void show_all_points(intrusive_list_t *list) {
    intrusive_node_t *current = list->head;

    while (current) {
        point_t *point = container_of(current, point_t, node);

        printf("(%d %d)", point->x, point->y);
        if (current->next)
            printf(" ");

        current = current->next;
    }

    printf("\n");
}

/**
 * Basically, clears the list.
 */
static void remove_all_points(intrusive_list_t *list) {
    intrusive_node_t *current = list->head;
    while (current) {
        point_t *point = container_of(current, point_t, node);
        intrusive_node_t *next = current->next;

        remove_node(list, current);
        free(point);

        current = next;
    }
}

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

/**
 * Add point command.
 * @return 0 on success otherwise -1
 */
static int handle_add(intrusive_list_t *list, int argc, char **argv) {
    if (argc != 3)
        return -1;
    int a, b;
    if (read_int(argv[1], &a) == -1)
        return -1;
    if (read_int(argv[2], &b) == -1)
        return -1;

    add_point(list, a, b);

    return 0;
}

/**
 * Remove points command.
 * @return 0 on success otherwise -1
 */
static int handle_rm(intrusive_list_t *list, int argc, char **argv) {
    if (argc != 3)
        return -1;
    int a, b;
    if (read_int(argv[1], &a) == -1)
        return -1;
    if (read_int(argv[2], &b) == -1)
        return -1;

    remove_point(list, a, b);

    return 0;
}

/**
 * Prints contents of the list.
 * @return 0 on success otherwise -1
 */
static int handle_print(intrusive_list_t *list, int argc, char **argv) {
    if (argc != 1)
        return -1;

    show_all_points(list);

    return 0;
}

/**
 * Remove all points command.
 * @return 0 on success otherwise -1
 */
static int handle_rma(intrusive_list_t *list, int argc, char **argv) {
    if (argc != 1)
        return -1;

    remove_all_points(list);

    return 0;
}

/**
 * Print length of the list command.
 * @return 0 on success otherwise -1
 */
static int handle_len(intrusive_list_t *list, int argc, char **argv) {
    if (argc != 1)
        return -1;

    printf("%d\n", (int) get_length(list));

    return 0;
}

/**
 * @param cmd command to split into tokens
 * @param argc pointer to argc
 * @param argv pointer to argv
 */
static void tokenize_cmd(char *cmd, int *argc, char ***argv) {
    // Trim new-line
    if (*cmd != 0) {
        char *back = cmd + (strlen(cmd) - 1);
        if (*back == '\n' || *back == '\r')
            *back = '\0';
    }

    *argc = 0;

    // Surely enough
    *argv = (char **) malloc((strlen(cmd) + 1) * sizeof(char *));
    assert(*argv);

    static const char *del = "\t ";
    char *token = strtok(cmd, del);
    while (token != NULL) {
        (*argv)[*argc] = token;
        ++(*argc);
        token = strtok(NULL, del);
    }
}

/**
 * @param cmd
 * @return 0 on success, 1 on exit cmd, otherwise -1
 */
static int handle_cmd(char *cmd, intrusive_list_t *list) {
    enum {
        N = 5
    };

    static const char *exit_cmd = "exit";
    static const char *cmd_names[N] = {
            "add", "rm", "print", "len", "rma"
    };
    static int (*cmd_handlers[N])(intrusive_list_t *, int, char **) = {
            &handle_add, &handle_rm, &handle_print,
            &handle_len, &handle_rma
    };

    int argc;
    char **argv;
    tokenize_cmd(cmd, &argc, &argv);


    int res = -1;

    if (argc == 0) // It's absolutely ok
        res = 0;
    else if (strcmp(argv[0], exit_cmd) == 0)
        res = 1;
    else
        for (size_t i = 0; i < N; ++i)
            if (strcmp(argv[0], cmd_names[i]) == 0) {
                res = (*cmd_handlers[i])(list, argc, argv);
                break;
            }

    free(argv);
    return res;
}

int main(void) {
    intrusive_list_t list;
    init_list(&list);

    enum {
        buffer_size = 100
    };
    char buffer[buffer_size];

    while (true) {
        fgets(buffer, buffer_size, stdin);
        int result = handle_cmd(buffer, &list);
        if (result == 1)
            break;
        else if (result == -1)
            printf("Unknown command\n");
    }

    remove_all_points(&list);

    return 0;
}
