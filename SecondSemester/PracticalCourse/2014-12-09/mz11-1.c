#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <wait.h>
#include <stdlib.h>
#include <ctype.h>

enum {
    SIZE = 1010
};

int parent, child_one, child_two;
char *shm;
FILE *file;
ssize_t count = 0;

void
handler (int signo)
{
    if (signo == SIGUSR1) {
        if (parent == getpid()) {
            ++count;
            kill(child_two, signo);
        } else {
            ssize_t len = strlen(shm);
            while (isspace(shm[len - 1])) {
                len -= 1;
            }
            printf("%zd\n", len);
            kill(parent, SIGUSR2);
        }
    } else if (signo == SIGUSR2) {
        if (parent == getpid()) {
            kill(child_one, signo);
        } else {
            shm = fgets(shm, SIZE, file);
            if (shm == NULL) {
                kill(parent, SIGIO);
                exit(0);
            }
            kill(parent, SIGUSR1);
        }
    } else if (signo == SIGIO) {
        if (parent == getpid()) {
            kill(child_two, signo);
        } else {
            exit(0);
        }
    }
}

int
main (int argc, char **argv)
{
    setbuf(stdout, NULL);

    ssize_t key;
    sscanf(argv[1], "%zd", &key);
    int id = shmget(key, SIZE, IPC_CREAT | 0666);
    shm = shmat(id, NULL, 0);

    file = fopen(argv[2], "r");
    parent = getpid();

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGIO, handler);

    child_one = fork();

    if (!child_one) {
        while (true) {
            pause();
        }
    }

    child_two = fork();
    kill(child_one, SIGUSR2);

    if (!child_two) {
        while (true) {
            pause();
        }
    }

    wait(NULL);
    wait(NULL);

    printf("%zd\n", count);
    shmctl(id, IPC_RMID, NULL);

    return 0;
}
