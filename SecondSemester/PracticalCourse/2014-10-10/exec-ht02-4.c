#include "ht02-4.c"

#include <stdio.h>

typedef const unsigned char *String;

int
main(void)
{
    String a = "a";
    String b = "b";
    String c = "c";
    String arr_one[3] = {a, b, c};
    printf("[a, b, c] <- a : %d\n", bsearchstr(arr_one, 3, a));
    printf("[a, b, c] <- b : %d\n", bsearchstr(arr_one, 3, b));
    printf("[a, b, c] <- c : %d\n", bsearchstr(arr_one, 3, c));
    printf("[a, b, c] <- d : %d\n", bsearchstr(arr_one, 3, "d"));
    String arr_two[0] = {};
    printf("[] <- b : %d\n", bsearchstr(arr_two, 0, b));
    String arr_three[1] = {a};
    printf("[a] <- a : %d\n", bsearchstr(arr_three, 1, a));
    printf("[a] <- b : %d\n", bsearchstr(arr_three, 1, b));

    return 0;
}

