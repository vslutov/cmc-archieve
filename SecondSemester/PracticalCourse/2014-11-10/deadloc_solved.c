#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int
main(void)
{
    int fd[2];
    pipe(fd);

    if (!fork()) {
        close(fd[0]);
        int val = 0;
        write(fd[1], &val, sizeof(val));
        _exit(0);
    } else {
        close(fd[1]);
        int val;
        while (read(fd[0], &val, sizeof(val)) != 0) {
            printf("%d\n", val);
        }
        wait(NULL);
    }
}

