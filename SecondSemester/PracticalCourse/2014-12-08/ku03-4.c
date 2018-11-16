#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

unsigned char x = 0;
int bits = 0;
int parent, child_one, child_two;
int fdr;

void
handler(int signo)
{
    if (signo == SIGUSR2 || signo == SIGUSR1) {
        x = x | ((signo == SIGUSR2) << bits);
        ++bits;
        if (bits == 8) {
            write(1, &x, sizeof(x));
            bits = 0;
            x = 0;
        }
        kill(parent, SIGALRM);
    } else if (signo == SIGIO) {
        exit(0);
    } else if (signo == SIGALRM) {
        if (parent == getpid()) {
            kill(child_two, SIGALRM);
        } else {
            if (bits < 8) {
                int bit = x & (1 << bits);
                signo = bit ? SIGUSR2 : SIGUSR1;
                kill(child_one, signo);
                ++ bits;
            } else if (read(fdr, &x, sizeof(x)) > 0) {
                bits = 0;
                handler(signo);
            } else {
                kill(child_one, SIGIO);
                exit(0);
            }
        }
    }
}

int
main(int argc, char **argv)
{
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGIO, handler);
    signal(SIGALRM, handler);

    sigset_t alrmset;
    sigemptyset(&alrmset);
    sigaddset(&alrmset, SIGALRM);
    sigprocmask(SIG_BLOCK, &alrmset, NULL);

    char *filename = argv[1];

    parent = getpid();

    child_one = fork();

    if (!child_one) {
        while (true) {
            pause();
        }
    }

    child_two = fork();

    sigprocmask(SIG_UNBLOCK, &alrmset, NULL);

    if (!child_two) {
        bits = 8;
        fdr = open(filename, O_RDONLY, 0777);
        handler(SIGALRM);
        while (true) {
            pause();
        }
    }

    wait(NULL);
    wait(NULL);
    return 0;
}
