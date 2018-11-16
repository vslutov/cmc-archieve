#include "ht02-2.c"

#include <stdio.h>

int
main(void)
{
    char s[500];
    while (scanf("%s", s) == 1) {
        int result = parse_rwx_permissions(s);
        if (result > 0) {
            printf("%s: 0%o\n", s, result);
        } else {
            printf("%s: %i\n", s, result);
        }
    }

    return 0;
}

