#include <stdbool.h>
#include <stdio.h>

int
main(void)
{
    unsigned int s;
    char c[10000];

    bool used[1024];
    for (int i = 0; i < 1024; ++i) {
        used[i] = false;
    }

    while (scanf("%x", &s) > 0) {
        fgets(c, sizeof(c), stdin);

        s >>= 22;
        used[s] = true;
    }

    int size = 1;
    for (int i = 0; i < 1024; ++i) {
        if (used[i]) {
            ++size;
        }
    }

    printf("%d\n", size * 4 * 1024);

    return 0;
}

