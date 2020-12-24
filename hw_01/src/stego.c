#include "stego.h"

static inline stego_err_t stego_get_channel_pnt(const bmp_t *bmp, bmp_pos_t pos,
                                                bmp_channel_t channel, uint8_t **ch) {
    rgb_triple_t *pxl;

    bmp_err_t bmp_err = get_pixel_in_bmp(bmp, pos, &pxl);
    if (bmp_err != BMP_OK)
        return STEGO_ILLEGAL_ARGUMENTS;

    *ch = (uint8_t *) pxl + channel;

    return STEGO_OK;
}

stego_err_t put_bit_into_bmp(bmp_t *dst, bmp_pos_t pos, bmp_channel_t channel, bool bit) {
    uint8_t *ch = NULL;

    stego_err_t stego_err = stego_get_channel_pnt(dst, pos, channel, &ch);
    if (stego_err != STEGO_OK) return stego_err;

    static const uint8_t mask = 0xFE; // All except least significant
    *ch = ((*ch & mask) | bit);

    return STEGO_OK;
}

stego_err_t get_bit_from_bmp(const bmp_t *src, bmp_pos_t pos, bmp_channel_t channel, bool *bit) {
    uint8_t *ch = NULL;

    stego_err_t stego_err = stego_get_channel_pnt(src, pos, channel, &ch);
    if (stego_err != STEGO_OK) return stego_err;

    *bit = (*ch & 1);

    return STEGO_OK;
}

#define BITS_PER_LETTER 5
#define LETTER_LUT_SIZE 30
static char letter_decoding_lut[LETTER_LUT_SIZE];
static int letter_encoding_lut[127];

static inline void build_letter_lut(void) {
    letter_encoding_lut[(int) '\0'] = 0;
    letter_decoding_lut[0] = '\0';

    int latin = 'Z' - 'A' + 1;
    for (int c = 'A', i = 1; c <= 'Z'; ++c, ++i) {
        letter_encoding_lut[c] = i;
        letter_decoding_lut[i] = (char) c;
    }
    char add_chs[] = {' ', '.', ','};
    for (int i = 0; i < 3; ++i) {
        char c = add_chs[i];
        letter_encoding_lut[(int) c] = 1 + latin + i;
        letter_decoding_lut[1 + latin + i] = c;
    }
}

static inline int encode_letter(char c) {
    return letter_encoding_lut[(int) c];
}

static inline char decode_letter(int i) {
    return letter_decoding_lut[i];
}

static int read_key_row(FILE *key_file, bmp_pos_t *pos, bmp_channel_t *channel) {
    char ch;
    if (fscanf(key_file, "%d %d %c", &pos->x, &pos->y, &ch) != 3)
        return 1;

    switch (ch) {
        case 'R':
            (*channel) = BMP_CHANNEL_R;
            break;
        case 'G':
            (*channel) = BMP_CHANNEL_G;
            break;
        case 'B':
            (*channel) = BMP_CHANNEL_B;
            break;
        default:
            return 1;
    }
    return 0;
}

static stego_err_t put_letter_in_bmp(bmp_t *bmp, FILE *key_file, int letter) {
    int encoded = encode_letter((char) letter);

    for (int i = 0; i < BITS_PER_LETTER; ++i) {
        bool bit = ((encoded >> i) & 1);

        bmp_pos_t pos;
        bmp_channel_t channel;

        if (read_key_row(key_file, &pos, &channel) != 0)
            return STEGO_ERR_READ_KEY;

        if (put_bit_into_bmp(bmp, pos, channel, bit) != STEGO_OK)
            return STEGO_ILLEGAL_ARGUMENTS;
    }

    return STEGO_OK;
}

stego_err_t write_msg_into_bmp(bmp_t *dst, FILE *key_file, FILE *msg_file) {
    stego_err_t stego_err;

    int letter;
    while ((letter = getc(msg_file)) != EOF)
        if ((stego_err = put_letter_in_bmp(dst, key_file, letter)) != STEGO_OK)
            return stego_err;

    // Put zero byte additionally.
    // We don't check for errors to write
    // if only we have more space in key.
    put_letter_in_bmp(dst, key_file, '\0');

    return STEGO_OK;
}

stego_err_t read_msg_from_bmp(const bmp_t *src, FILE *key_file, FILE *msg_file) {
    bmp_pos_t pos;
    bmp_channel_t channel;

    int bit_cnt = 0;
    uint8_t cur_encoded = 0;

    while (read_key_row(key_file, &pos, &channel) == 0) {
        bool bit = 0;

        stego_err_t stego_err = get_bit_from_bmp(src, pos, channel, &bit);
        if (stego_err != STEGO_OK) return stego_err;

        cur_encoded |= (bit << bit_cnt);
        if (++bit_cnt % BITS_PER_LETTER == 0) {
            char letter = decode_letter(cur_encoded);

            if (letter == '\0')
                break;

            if (putc(letter, msg_file) == EOF)
                return STEGO_ERR_WRITE_MSG;

            cur_encoded = 0;
            bit_cnt = 0;
        }
    }

    return STEGO_OK;
}

void init_stego(void) {
    build_letter_lut();
}
