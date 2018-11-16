#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <inttypes.h>

int maxn;
int num;

FILE *fr, *fw;
int other;

void
handler(int signo)
{
    intmax_t a;
    if (fscanf(fr, "%jd", &a) > 0 && a < maxn) {
        printf("%d %jd\n", num, a);
        ++a;
        fprintf(fw, "%jd\n", a);
        kill(other, SIGUSR1);
    } else {
        exit(0);
    }
}

void
none (int signo)
{
    signo = signo;
}

int
main(int argc, char **argv)
{
    setbuf(stdout, NULL);

    int fd[2];
    pipe(fd);

    fr = fdopen(fd[0], "r");
    fw = fdopen(fd[1], "w");

    sscanf(argv[1], "%d", &maxn);

    signal(SIGUSR1, handler);
    signal(SIGUSR2, none);

    if (!(other = fork())) {
        other = getppid();
        num = 1;

        while (true) {
            pause();
        }
    } else if (!fork()) {
        num = 2;
        usleep(1);
        usleep(1);
        usleep(1);
        usleep(1);
        usleep(1);

        intmax_t val = 1;
        fprintf(fw, "%jd\n", val);
        kill(other, SIGUSR1);

        while (true) {
            pause();
        }
    } else {
        wait(NULL);
        wait(NULL);

        printf("Done\n");
    }

    return 0;
}

