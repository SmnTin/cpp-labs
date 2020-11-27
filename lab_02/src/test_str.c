#include "test_str.h"

#include "str.h"

#include "assert.h"
#include "stdlib.h"

void test_strlen_on_small() {
    char *s = "abacaba";
    assert(my_strlen(s) == 7);
}

void test_strlen_on_empty() {
    char *s = "";
    assert(my_strlen(s) == 0);
}

void test_strlen_on_large() {
    const size_t N = 10000;

    char *s = (char *)calloc(N, sizeof(char));
    for (size_t i = 0; i < N; ++i)
        *(s + i) = 'f';
    s[N] = '\0';

    size_t len = my_strlen(s);
    free(s);

    assert(len == N);
}

void test_strlen() {
    test_strlen_on_small();
    test_strlen_on_empty();
    test_strlen_on_large();
}

void test_strcpy_simple() {
    char *s2 = "funf";
    char s1[my_strlen(s2) + 1];

    assert(my_strcmp(my_strcpy(s1, s2), s2) == 0);
}

void test_strcpy_complex() {
    char res[my_strlen("aaa") + 1];
    my_strcpy(my_strcpy(res, "aaa"), "bbb");

    assert(my_strcmp(res, "bbb") == 0);
}

void test_strcpy() {
    test_strcpy_simple();
    test_strcpy_complex();
}

void test_strcat() {
    char s1[8] = "obi-";
    char *s2 = "wan";
    char *expected = "obi-wan";

    assert(my_strcmp(my_strcat(s1, s2), expected) == 0);
}

void test_strcmp_on_prefix_1() {
    char *s1 = "aba";
    char *s2 = "abaca";

    assert(my_strcmp(s1, s2) < 0);
}

void test_strcmp_on_prefix_2() {
    char *s1 = "ban-hammer";
    char *s2 = "ban";

    assert(my_strcmp(s1, s2) > 0);
}

void test_strcmp_on_less() {
    char *s1 = "aaaaa";
    char *s2 = "aab";

    assert(my_strcmp(s1, s2) < 0);
}

void test_strcmp_on_greater() {
    char *s1 = "bravit";
    char *s2 = "best";

    assert(my_strcmp(s1, s2) > 0);
}

void test_strcmp_on_equal() {
    char *s1 = "anime";
    char *s2 = "anime";

    assert(my_strcmp(s1, s2) == 0);
}

void test_strcmp() {
    test_strcmp_on_prefix_1();
    test_strcmp_on_prefix_2();
    test_strcmp_on_less();
    test_strcmp_on_greater();
    test_strcmp_on_equal();
}