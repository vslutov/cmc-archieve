#include <stdio.h>
#include <unistd.h>

int
main(void)
{
    int fd[2];
    pipe(fd);

    if (!fork()) {
        int val = 0;
        write(fd[1], &val, sizeof(val));
        _exit(0);
    } else {
        int val;
        while (read(fd[0], &val, sizeof(val)) != 0) {
            printf("%d\n", val);
        }
        close(fd[1]);
        close(fd[0]);
        wait(NULL);
    }
}

