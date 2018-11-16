#include "ht02-5.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const unsigned char *String;

int
compar(const void *p1, const void *p2, void *user) {
    return strcmp(*(char **)p1, *(char **)p2);
}

int
main(void)
{
    String a = "a";
    String b = "b";
    String c = "c";
    String d = "d";

    String arr_one[5] = {a, b, b, b, c};
    ssize_t low, high;
    int res;
    res = bsearch2(&a, arr_one, 5, sizeof(*arr_one), compar, NULL, &low, &high);
    printf("[a, b, b, b, c] <- a : %d %zd %zd\n", res, low, high);
    res = bsearch2(&b, arr_one, 5, sizeof(*arr_one), compar, NULL, &low, &high);
    printf("[a, b, b, b, c] <- b : %d %zd %zd\n", res, low, high);
    res = bsearch2(&c, arr_one, 5, sizeof(*arr_one), compar, NULL, &low, &high);
    printf("[a, b, b, b, c] <- c : %d %zd %zd\n", res, low, high);
    res = bsearch2(&d, arr_one, 5, sizeof(*arr_one), compar, NULL, &low, &high);
    printf("[a, b, b, b, c] <- d : %d %zd %zd\n", res, low, high);
    String arr_two[0] = {};
    res = bsearch2(&b, arr_two, 0, sizeof(*arr_two), compar, NULL, &low, &high);
    printf("[] <- b : %d %zd %zd\n", res, low, high);
    String arr_three[1] = {b};
    res = bsearch2(&a, arr_three, 1, sizeof(*arr_three), compar, NULL, &low, &high);
    printf("[b] <- a : %d %zd %zd\n", res, low, high);
    res = bsearch2(&b, arr_three, 1, sizeof(*arr_three), compar, NULL, &low, &high);
    printf("[b] <- b : %d %zd %zd\n", res, low, high);
    res = bsearch2(&c, arr_three, 1, sizeof(*arr_three), compar, NULL, &low, &high);
    printf("[b] <- c : %d %zd %zd\n", res, low, high);
    String arr_four[5] = {a, a, b, d, d};
    res = bsearch2(&c, arr_four, 5, sizeof(*arr_four), compar, NULL, &low, &high);
    printf("[a, a, b, d, d] <- c : %d %zd %zd\n", res, low, high);

    return 0;
}

