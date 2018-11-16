#include "ht01-1.c"

#include <stdio.h>

enum {
    MAX_SIZE = 100
};

int array[MAX_SIZE];

int
main(void)
{
    size_t count = 0;

    for (size_t i = 0; scanf("%d", &array[i]) > 0; ++i) {
        ++count;
    }

    printf("%d\n", getmax2(count, array));

    return 0;
}

