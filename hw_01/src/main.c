#include "bmp.h"
#include "stego.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

static void print_bmp_err_msg(bmp_err_t bmp_err) {
    switch (bmp_err) {
        case BMP_ERR_MEM_ALLOC:
            fprintf(stderr, "An error occurred during memory allocation.\n");
            break;
        case BMP_ERR_ILLEGAL_ARGS:
            fprintf(stderr, "Illegal arguments were passed to program.\n");
            break;
        case BMP_ERR_FILE_READ:
            fprintf(stderr, "An error occurred during reading input file.\n");
            break;
        case BMP_ERR_FILE_WRITE:
            fprintf(stderr, "An error occurred during writing to output file.\n");
            break;
        default:
            break;
    }
}

static void print_stego_err_msg(stego_err_t stego_err) {
    switch (stego_err) {
        case STEGO_ERR_READ_KEY:
            fprintf(stderr, "An error occurred during reading file containing key.\n");
            break;
        case STEGO_ILLEGAL_ARGUMENTS:
            fprintf(stderr, "Illegal arguments were passed to program.\n");
            break;
        case STEGO_ERR_READ_BMP:
            fprintf(stderr, "An error occurred during reading bmp file.\n");
            break;
        case STEGO_ERR_WRITE_MSG:
            fprintf(stderr, "An error occurred during writing to message file.\n");
            break;
        default:
            break;
    }
}

static int open_file(const char *file_name, const char *readable_name,
                     const char *mode, FILE **in_file) {
    *in_file = fopen(file_name, mode);
    if (!*in_file) {
        fprintf(stderr, "Could not open %s file.\n", readable_name);
        return 1;
    }
    return 0;
}

#define handle_bmp_err(bmp_err) \
        if (bmp_err != BMP_OK) { \
            print_bmp_err_msg(bmp_err); \
            goto error; \
        }

#define handle_stego_err(stego_err) \
        if (stego_err != STEGO_OK) { \
            print_stego_err_msg(stego_err); \
            goto error; \
        }

static int handle_crop_rotate(int argc, char **argv) {
    if (argc != 8) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return 1;
    }

    char *in_file_name = argv[2];
    char *out_file_name = argv[3];

    bmp_err_t bmp_err;
    bmp_rect_t crop_rect;

    crop_rect.pos.x = atoi(argv[4]);
    crop_rect.pos.y = atoi(argv[5]);
    crop_rect.size.width = atoi(argv[6]);
    crop_rect.size.height = atoi(argv[7]);

    bmp_t *orig = NULL;
    bmp_t *cropped = NULL;
    bmp_t *rotated = NULL;

    FILE *in_file = NULL;
    FILE *out_file = NULL;

    if (open_file(in_file_name, "input", "rb", &in_file) != 0 ||
        open_file(out_file_name, "output", "wb", &out_file) != 0)
        goto error;

    if ((bmp_err = load_bmp(&orig, in_file)) != BMP_OK ||
        (bmp_err = crop_bmp(&cropped, orig, crop_rect)) != BMP_OK ||
        (bmp_err = rotate_bmp(&rotated, cropped, BMP_ROT_CLOCKWISE_90)) != BMP_OK ||
        (bmp_err = save_bmp(rotated, out_file)) != BMP_OK) {

        print_bmp_err_msg(bmp_err);
        goto error;
    }

    int err_code = 0;
    goto clear;

    error:
    err_code = 1;

    clear:
    if (in_file) fclose(in_file);
    if (out_file) fclose(out_file);
    if (orig) free_bmp(orig);
    if (cropped) free_bmp(cropped);
    if (rotated) free_bmp(rotated);

    return err_code;
}

static int handle_insert(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return 1;
    }

    char *in_file_name = argv[2];
    char *out_file_name = argv[3];
    char *key_file_name = argv[4];
    char *msg_file_name = argv[5];

    bmp_err_t bmp_err;
    bmp_t *bmp = NULL;

    FILE *in_file = NULL;
    FILE *out_file = NULL;
    FILE *key_file = NULL;
    FILE *msg_file = NULL;

    if (open_file(in_file_name, "input", "rb", &in_file) != 0 ||
        open_file(key_file_name, "key", "r", &key_file) != 0 ||
        open_file(msg_file_name, "msg", "r", &msg_file) != 0 ||
        open_file(out_file_name, "out", "wb", &out_file) != 0)
        goto error;

    bmp_err = load_bmp(&bmp, in_file);
    handle_bmp_err(bmp_err)

    stego_err_t stego_err = write_msg_into_bmp(bmp, key_file, msg_file);
    handle_stego_err(stego_err)

    bmp_err = save_bmp(bmp, out_file);
    handle_bmp_err(bmp_err)

    int err_code = 0;
    goto clear;

    error:
    err_code = 1;

    clear:
    if (bmp) free_bmp(bmp);
    if (in_file) fclose(in_file);
    if (out_file) fclose(out_file);
    if (key_file) fclose(key_file);
    if (msg_file) fclose(msg_file);

    return err_code;
}

static int handle_extract(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return 1;
    }
    char *in_file_name = argv[2];
    char *key_file_name = argv[3];
    char *msg_file_name = argv[4];

    bmp_err_t bmp_err;
    bmp_t *bmp = NULL;

    FILE *in_file = NULL;
    FILE *key_file = NULL;
    FILE *msg_file = NULL;

    if (open_file(in_file_name, "input", "rb", &in_file) != 0 ||
        open_file(key_file_name, "key", "r", &key_file) != 0 ||
        open_file(msg_file_name, "msg", "w", &msg_file) != 0)
        goto error;

    bmp_err = load_bmp(&bmp, in_file);
    handle_bmp_err(bmp_err)

    stego_err_t stego_err = read_msg_from_bmp(bmp, key_file, msg_file);
    handle_stego_err(stego_err)

    int err_code = 0;
    goto clear;

    error:
    err_code = 1;

    clear:
    if (bmp) free_bmp(bmp);
    if (in_file) fclose(in_file);
    if (key_file) fclose(key_file);
    if (msg_file) fclose(msg_file);

    return err_code;
}

int main(int argc, char **argv) {
    init_stego();

    if (argc < 2) {
        fprintf(stderr, "Not enough arguments.");
        return 1;
    } else if (strcmp(argv[1], "crop-rotate") == 0) {
        return handle_crop_rotate(argc, argv);
    } else if (strcmp(argv[1], "insert") == 0) {
        return handle_insert(argc, argv);
    } else if (strcmp(argv[1], "extract") == 0) {
        return handle_extract(argc, argv);
    } else {
        fprintf(stderr, "Unknown command.");
        return 1;
    }
}
