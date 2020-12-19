#include "bmp.h"

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

static int handle_crop_rotate(int argc, char **argv) {
    if (argc != 8) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return 1;
    }
    char *in_file_name = argv[2];
    char *out_file_name = argv[3];

    bmp_rect_t crop_rect;

    crop_rect.pos.x = atoi(argv[4]);
    crop_rect.pos.y = atoi(argv[5]);
    crop_rect.size.width = atoi(argv[6]);
    crop_rect.size.height = atoi(argv[7]);

    FILE *in_file = fopen(in_file_name, "rb");
    if (!in_file) {
        fprintf(stderr, "Could not open input file.\n");
        return 1;
    }

    bmp_err_t bmp_err;

    bmp_t *orig;
    bmp_err = load_bmp(&orig, in_file);

    fclose(in_file);
    if (bmp_err != BMP_OK) {
        print_bmp_err_msg(bmp_err);
        return 1;
    }

    bmp_t *cropped;
    bmp_err = crop_bmp(&cropped, orig, crop_rect);
    if (bmp_err != BMP_OK) {
        print_bmp_err_msg(bmp_err);
        free_bmp(orig);
        return 1;
    }

    bmp_t *rotated;
    bmp_err = rotate_bmp(&rotated, cropped, BMP_ROT_CLOCKWISE_90);
    if (bmp_err != BMP_OK) {
        print_bmp_err_msg(bmp_err);
        free_bmp(orig);
        free_bmp(cropped);
        return 1;
    }

    FILE *out_file = fopen(out_file_name, "wb");
    if (!out_file) {
        fprintf(stderr, "Could not open output file.\n");
        free_bmp(orig);
        free_bmp(cropped);
        free_bmp(rotated);
        return 1;
    }

    bmp_err = save_bmp(rotated, out_file);
    if (bmp_err != BMP_OK) {
        print_bmp_err_msg(bmp_err);
        free_bmp(orig);
        free_bmp(cropped);
        free_bmp(rotated);
        return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments.");
        return 1;
    } else if (strcmp(argv[1], "crop-rotate") == 0) {
        return handle_crop_rotate(argc, argv);
    } else {
        fprintf(stderr, "Unknown command.");
        return 1;
    }
}
