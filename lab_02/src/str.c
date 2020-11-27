#include "str.h"

char *my_strcpy(char *restrict dest,
                const char *restrict src) {
    char *res = dest;
    while ((*dest++ = *src++));

    return res;
}

char *my_strcat(char *restrict dest,
                const char *restrict src) {
    my_strcpy(dest + my_strlen(dest), src);
    return dest;
}

int my_strcmp(const char *restrict s1,
              const char *restrict s2) {
    for (; *s1 && *s1 == *s2; ++s1, ++s2);
    return *s1 - *s2;
}

size_t my_strlen(const char *restrict s) {
    size_t len = 0;
    while (*s++) ++len;
    return len;
}
