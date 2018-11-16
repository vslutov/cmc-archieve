#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

int mode = 0;

void
handler(int signo)
{
    mode = signo != SIGUSR1;
}

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    printf("%d\n", getpid());

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    intmax_t a;
    while (scanf("%jd", &a) > 0) {
        if (mode == 0) {
            a = -a;
        } else {
            a = a * a;
        }
        printf("%jd\n", a);
    }
}

