#include <stdio.h>
#include <stdlib.h>

typedef const char *String;

String p[8] = {
    "---",
    "--x",
    "-w-",
    "-wx",
    "r--",
    "r-x",
    "rw-",
    "rwx"
};

int
main(int argc, char **argv)
{
    for (size_t i = 1; i < argc; ++i) {
        int perms;
        sscanf(argv[i], "%o", &perms);

        for (size_t j = 0; j < 3; ++j) {
            int perm = (perms & (7 << 6)) >> 6;
            printf("%s", p[perm]);

            perms <<= 3;
        }

        printf("\n");
    }

    return 0;
}
