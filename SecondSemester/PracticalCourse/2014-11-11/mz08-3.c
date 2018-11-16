#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

int maxn;
int num;

void
get_val(int fdr, int fdw)
{
    int val;

    if (!read(fdr, &val, sizeof(val))) {
        exit(0);
    }

    if (val >= maxn) {
        exit(0);
    } else {
        printf("%d %d\n", num, val);
        ++val;
        write(fdw, &val, sizeof(val));
    }
}

int
main(int argc, char **argv)
{
    setbuf(stdout, NULL);

    int fd[2][2];
    pipe(fd[0]);
    pipe(fd[1]);

    sscanf(argv[1], "%d", &maxn);

    if (!fork()) {
        num = 1;

        close(fd[0][0]);
        close(fd[1][1]);

        while (true) {
            get_val(fd[1][0], fd[0][1]);
        }

    } else if (!fork()) {
        num = 2;

        close(fd[0][1]);
        close(fd[1][0]);

        int val = 1;
        write(fd[1][1], &val, sizeof(val));

        while (true) {
            get_val(fd[0][0], fd[1][1]);
        }

    } else {
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[1][1]);

        wait(NULL);
        wait(NULL);

        printf("Done\n");
    }

    return 0;
}

