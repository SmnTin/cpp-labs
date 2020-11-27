#ifndef LAB_02_STR_H
#define LAB_02_STR_H

// Only for size_t
#include "stddef.h"

char *my_strcpy(char *restrict dest,
                const char *restrict src);

char *my_strcat(char *restrict dest,
                const char *restrict src);

int my_strcmp(const char *restrict s1,
              const char *restrict s2);

size_t my_strlen(const char *s);

#endif //LAB_02_STR_H
