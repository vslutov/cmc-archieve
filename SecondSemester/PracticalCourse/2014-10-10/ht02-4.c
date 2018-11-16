#include <string.h>

int
bsearchstr(
    const unsigned char * const *arr,
    int size,
    const unsigned char *key) {

    size_t i = 0;
    size_t j = size;

    // There are no arr[k] <= key, when k >= j

    while (j - i > 1) {
        size_t k = (i + j) / 2;
        int cmp = strcmp(arr[k], key);

        if (cmp > 0) {
            j = k;
        } else {
            i = k;
        }
    }

    if (size > 0 && strcmp(arr[i], key) == 0) {
        return i;
    } else {
        return -1;
    }

}

