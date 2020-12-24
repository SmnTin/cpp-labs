#ifndef HW_01_STEGO_H
#define HW_01_STEGO_H

#include "bmp.h"

typedef enum {
    BMP_CHANNEL_R = 2,
    BMP_CHANNEL_G = 1,
    BMP_CHANNEL_B = 0,
} bmp_channel_t;

typedef enum {
    STEGO_OK = 0,
    STEGO_ILLEGAL_ARGUMENTS = 1,
    STEGO_ERR_READ_KEY = 2,
    STEGO_ERR_READ_BMP = 3,
    STEGO_ERR_WRITE_MSG = 4
} stego_err_t;

void init_stego(void);
bmp_err_t put_bit_into_bmp(bmp_t *dst, bmp_pos_t pos, bmp_channel_t channel, bool bit);
bmp_err_t get_bit_from_bmp(const bmp_t *src, bmp_pos_t pos, bmp_channel_t channel, bool *bit);
stego_err_t write_msg_into_bmp(bmp_t *dst, FILE *key_file, FILE *msg_file);
stego_err_t read_msg_from_bmp(const bmp_t *src, FILE *key_file, FILE *msg_file);

#endif //HW_01_STEGO_H
