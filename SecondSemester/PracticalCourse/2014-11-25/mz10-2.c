#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool print = false;
size_t count = 0;

void
handler (int signo) {
    if (signo == SIGTERM) {
        exit(0);
    } else {
        ++ count;
        if (count == 4) {
            exit(0);
        } else {
            print = true;
        }
    }
}

bool
is_prime(intmax_t x)
{
    bool ok = true;
    if (x == 1) {
        ok = false;
    }
    for (intmax_t j = 2; j * j <= x; ++j) {
        if (x % j == 0) {
            ok = false;
            break;
        }
    }
    return ok;
}

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    intmax_t low, high;
    sscanf(argv[1], "%jd", &low);
    sscanf(argv[2], "%jd", &high);

    intmax_t last = 0;
    for (intmax_t i = low; i < high; ++i) {
        if (is_prime(i)) {
            last = i;
            break;
        }
    }

    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    printf("%d\n", getpid());

    for (intmax_t i = last; i < high; ++i) {
        if (is_prime(i)) {
            last = i;
        }
        if (print) {
            printf("%jd\n", last);
            print = false;
        }
    }

    printf("-1\n");
    return 0;
}

