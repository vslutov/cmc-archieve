#include <stdio.h>
#include <unistd.h>

int
main(void)
{
    int r = 0;
    while (r >= 0) {
        scanf("%d", &r);
        write(1, &r, sizeof(r));
    }

    return 0;
}
