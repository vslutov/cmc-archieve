#include <stdlib.h>

#include <stdio.h>

const void *
value_of(const void *base, ssize_t number, ssize_t size)
{
    return ((const char *) base) + size * number;
}

int
bsearch2(
    const void *key,
    const void *base,
    ssize_t nmemb,
    ssize_t size,
    int (*compar)(const void *p1, const void *p2, void *user),
    void *user,
    ssize_t *p_low,
    ssize_t *p_high)
{
    if (nmemb == 0) {
        *p_low = 0;
        *p_high = 0;
        return 0;
    }

    ssize_t i = 0;
    ssize_t j = nmemb;
    int cmp;

    while (j - i > 1) {
        ssize_t k = (i + j) / 2;
        cmp = compar(value_of(base, k, size), key, user);

        if (cmp > 0) {
            j = k;
        } else {
            i = k;
        }
    }

    cmp = compar(value_of(base, i, size), key, user);
    if (cmp == 0) {
        *p_high = j;
    } else if (cmp < 0) {
        *p_low = *p_high = j;
        return 0;
    } else {
        *p_low = *p_high = 0;
        return 0;
    }

    i = -1;
    j = nmemb - 1;

    while (j - i > 1) {
        ssize_t k = (i + j) / 2;
        cmp = compar(value_of(base, k, size), key, user);

        if (cmp < 0) {
            i = k;
        } else {
            j = k;
        }
    }

    *p_low = j;
    return 1;
}

