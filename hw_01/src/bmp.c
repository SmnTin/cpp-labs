#include "bmp.h"

#include "stdlib.h"
#include "math.h"
#include "stdbool.h"
#include "string.h"

struct __attribute__((packed)) BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits; // Bytes
};

struct __attribute__((packed)) BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

typedef struct {
    uint8_t b, g, r;
} __attribute__((packed)) rgb_triple_t;

struct __bmp {
    struct BITMAPFILEHEADER file_header;
    struct BITMAPINFOHEADER info_header;
    bmp_size_t size;
    size_t content_size;
    rgb_triple_t **data;
};

static inline size_t calc_bmp_content_size(bmp_size_t size) {
    size_t row_size = size.width * sizeof(rgb_triple_t);
    row_size += (4 - row_size % 4) % 4; // Data alignment
    return row_size * size.height;
}

static void **alloc_2d_array(size_t rows, size_t row_size, size_t content_size) {
    size_t pnt_arr_size = sizeof(void *) * rows;

    void *raw_data = malloc(pnt_arr_size + content_size);
    if (!raw_data)
        return NULL;

    void **pnt_arr = (void **) raw_data;
    char *content = (char *) raw_data + pnt_arr_size;

    for (size_t i = 0; i < rows; ++i, content += row_size)
        pnt_arr[i] = content;

    return raw_data;
}

static inline bmp_err_t read_file_header(bmp_t *bmp, FILE *in_file) {
    size_t read_items = fread(&bmp->file_header, sizeof(struct BITMAPFILEHEADER), 1, in_file);
    if (read_items != 1)
        return BMP_ERR_FILE_READ;
    return BMP_OK;
}

static inline bmp_err_t read_info_header(bmp_t *bmp, FILE *in_file) {
    size_t read_items = fread(&bmp->info_header, sizeof(struct BITMAPINFOHEADER), 1, in_file);
    if (read_items != 1)
        return BMP_ERR_FILE_READ;

    bmp->size.width = bmp->info_header.biWidth;
    bmp->size.height = abs(bmp->info_header.biHeight);
    bmp->content_size = bmp->info_header.biSizeImage;

    if (bmp->content_size == 0)
        bmp->content_size = calc_bmp_content_size(bmp->size);

    return BMP_OK;
}

static inline bmp_err_t read_pixel_data(bmp_t *bmp, FILE *in_file) {
    size_t row_size = bmp->content_size / bmp->size.height;

    void **data = alloc_2d_array(bmp->size.height, row_size, bmp->content_size);
    if (!data)
        return BMP_ERR_MEM_ALLOC;

    bmp->data = (rgb_triple_t **) data;

    void *pixel_data = data[0];
    int io_err = fseek(in_file, bmp->file_header.bfOffBits, SEEK_SET);

    if (io_err != 0) {
        free(data);
        return BMP_ERR_FILE_READ;
    }

    size_t read_items = fread(pixel_data, bmp->content_size, 1, in_file);
    if (read_items != 1) {
        free(data);
        return BMP_ERR_FILE_READ;
    }

    return BMP_OK;
}

#define load_bmp_handle_err(bmp_err) \
    if (bmp_err != BMP_OK) { \
        free(bmp); \
        return bmp_err; \
    }

bmp_err_t load_bmp(bmp_t **out_bmp, FILE *in_file) {
    // Ensuring we are at the beginning
    rewind(in_file);

    *out_bmp = NULL;
    bmp_t *bmp = malloc(sizeof(bmp_t));
    if (!bmp)
        return BMP_ERR_MEM_ALLOC;

    bmp_err_t bmp_err;

    bmp_err = read_file_header(bmp, in_file);
    load_bmp_handle_err(bmp_err)

    bmp_err = read_info_header(bmp, in_file);
    load_bmp_handle_err(bmp_err)

    bmp_err = read_pixel_data(bmp, in_file);
    load_bmp_handle_err(bmp_err)

    *out_bmp = bmp;
    return BMP_OK;
}

static inline bmp_err_t write_file_header(bmp_t *bmp, FILE *out_file) {
    // Writing pixels right after headers
    size_t content_pos =
            sizeof(struct BITMAPFILEHEADER) +
            sizeof(struct BITMAPINFOHEADER);
    bmp->file_header.bfOffBits = content_pos;

    size_t written_items = fwrite(&bmp->file_header, sizeof(struct BITMAPFILEHEADER), 1, out_file);
    if (written_items != 1)
        return BMP_ERR_FILE_WRITE;
    return BMP_OK;
}

static inline bmp_err_t write_info_header(bmp_t *bmp, FILE *out_file) {
    bmp->info_header.biHeight = bmp->size.height;
    bmp->info_header.biWidth = bmp->size.width;
    bmp->info_header.biSizeImage = bmp->content_size;

    size_t written_items = fwrite(&bmp->info_header, sizeof(struct BITMAPINFOHEADER), 1, out_file);
    if (written_items != 1)
        return BMP_ERR_FILE_WRITE;
    return BMP_OK;
}

static inline bmp_err_t write_pixel_data(bmp_t *bmp, FILE *out_file) {
    void *pixel_data = bmp->data[0];

    size_t written_items = fwrite(pixel_data, bmp->content_size, 1, out_file);
    if (written_items != 1)
        return BMP_ERR_FILE_WRITE;

    return BMP_OK;
}

bmp_err_t save_bmp(bmp_t *bmp, FILE *out_file) {
    // Ensuring we are at the beginning
    rewind(out_file);

    bmp_err_t bmp_err;

    bmp_err = write_file_header(bmp, out_file);
    if (bmp_err != 0) return bmp_err;

    bmp_err = write_info_header(bmp, out_file);
    if (bmp_err != 0) return bmp_err;

    bmp_err = write_pixel_data(bmp, out_file);
    if (bmp_err != 0) return bmp_err;

    int io_err = fflush(out_file);
    if (io_err != 0)
        return BMP_ERR_FILE_WRITE;

    return BMP_OK;
}

void free_bmp(bmp_t *bmp) {
    free(bmp->data);
    free(bmp);
}

static inline bmp_err_t create_bmp_with_size(bmp_t **dst, bmp_t *ref, bmp_size_t size) {
    bmp_t *bmp = malloc(sizeof(bmp_t));
    if (!bmp)
        return BMP_ERR_MEM_ALLOC;

    *bmp = *ref;

    bmp->size = size;
    bmp->content_size = calc_bmp_content_size(size);

    size_t row_size = bmp->content_size / size.height;
    bmp->data = (rgb_triple_t **) alloc_2d_array(size.height, row_size, bmp->content_size);
    if (!bmp->data) {
        free(bmp);
        return BMP_ERR_MEM_ALLOC;
    }

    *dst = bmp;
    return BMP_OK;
}

bmp_err_t crop_bmp(bmp_t **out_dst, bmp_t *src, bmp_rect_t region) {
    *out_dst = NULL;
    bmp_err_t bmp_err;

    if (region.pos.x < 0 || region.pos.y < 0 ||
        region.pos.x + region.size.width > src->size.width ||
        region.pos.y + region.size.height > src->size.height)
        return BMP_ERR_ILLEGAL_ARGS;

    // Convert top-down to bottom-up positioning
    region.pos.y = src->size.height - region.pos.y - region.size.height;

    bmp_t *dst;
    bmp_err = create_bmp_with_size(&dst, src, region.size);
    if (bmp_err != BMP_OK)
        return bmp_err;

    for (size_t row = 0; row < region.size.height; ++row)
        for (size_t col = 0; col < region.size.width; ++col)
            dst->data[row][col] = src->data[row + region.pos.y][col + region.pos.x];

    *out_dst = dst;
    return BMP_OK;
}

bmp_err_t clone_image(bmp_t **out_dst, bmp_t *src) {
    *out_dst = NULL;
    bmp_t *dst;

    bmp_err_t bmp_err = create_bmp_with_size(&dst, src, src->size);
    if (bmp_err != BMP_OK)
        return bmp_err;

    memcpy(dst->data, src->data, src->content_size);
    *out_dst = dst;
    return BMP_OK;
}

static inline bmp_err_t rotate_bmp_clockwise_90(bmp_t **out_dst, bmp_t *src) {
    *out_dst = NULL;
    bmp_t *dst;

    bmp_size_t rot_size;
    rot_size.width = src->size.height;
    rot_size.height = src->size.width;

    bmp_err_t bmp_err = create_bmp_with_size(&dst, src, rot_size);
    if (bmp_err != BMP_OK)
        return bmp_err;

    for (size_t row = 0; row < rot_size.height; ++row)
        for (size_t col = 0; col < rot_size.width; ++col)
            dst->data[row][col] = src->data[col][rot_size.height - row - 1];

    *out_dst = dst;
    return BMP_OK;
}

bmp_err_t rotate_bmp(bmp_t **dst, bmp_t *src, bmp_rot_t rot) {
    switch (rot) {
        case BMP_ROT_NONE:
            return clone_image(dst, src);
        case BMP_ROT_CLOCKWISE_90:
            return rotate_bmp_clockwise_90(dst, src);
        default:
            return BMP_ERR_ILLEGAL_ARGS;
    }
}
