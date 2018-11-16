#include <unisdt.h>

#include <stdio.h>
#include <stdlib.h>

int pid = 0;
int fd[2];


void
handler(int s)
{
    int v;
    read(fd[0], &v, sizeof(v));
    printf("%d %d\n", getpid(), v);
    ++ v;
    write(fd[1], &v, sizeof(v));
    kill(pid, SIGUSR1);
}

int
main(void)
{
    setbuf(stdout, NULL);
    pipe(fd);
    signal(SIGUSR1, handler);
    if (!(pid = fork())) {
        pid = getppid();
        while (1) {
            pause();
        }
    } else {
        int v = 0;
        write(fd[1], &v, sizeof(v));
        kill(pid, SIGUSR1);
        while(1) {
            pause();
        }
    }
}
