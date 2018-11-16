#include "ht01-2.c"

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    char *s;

    while((s = getline2(stdin)) != NULL) {
        fputs(s, stdout);
        free(s);
    }

    return 0;
}
