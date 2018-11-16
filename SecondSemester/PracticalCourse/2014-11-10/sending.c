#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

enum {
    MAXN = 20
};

int step;

int
get_val(int fdr, int fdw)
{
    int val;

    if (!read(fdr, &val, sizeof(val))) {
        _exit(0);
    }

    printf("%d %d\n", getpid(), val);

    val += step;
    if (val >= MAXN) {
        val = MAXN;
        close(fdr);
    }

    write(fdw, &val, sizeof(val));

    if (val == MAXN) {
        close(fdw);
    }

    return val;
}

int
main(int argc, char **argv)
{
    setbuf(stdout, NULL);

    int fd[2][2];
    pipe(fd[0]);
    pipe(fd[1]);

    int val;
    sscanf(argv[1], "%d", &val);

    if (!fork()) {
        close(fd[0][0]);
        close(fd[1][1]);

        step = 1;

        while (val < MAXN) {
            val = get_val(fd[1][0], fd[0][1]);
        }

    } else {
        close(fd[0][1]);
        close(fd[1][0]);

        write(fd[1][1], &val, sizeof(val));
        step = 2;

        while (val < MAXN) {
            val = get_val(fd[0][0], fd[1][1]);
        }

    }

    return 0;
}

