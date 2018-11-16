#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

typedef char Path[PATH_MAX];

void
traverse(const char *p, bool head)
{
    DIR *d = opendir(p);
    if (d == NULL) {
        return;
    }

    struct dirent *dd;

    while ((dd = readdir(d)) != NULL) {
        if (!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, "..")) {
            continue;
        }

        char buf[PATH_MAX];
        snprintf(buf, sizeof(buf), "%s/%s", p, dd->d_name);

        struct stat stb;
        if (stat(buf, &stb) < 0) { // Maybe file was deleted? [race condition]
            continue;
        }

        if (S_ISDIR(stb.st_mode)) {
            printf("cd %s\n", dd->d_name);
            traverse(buf, false);
        }
    }

    closedir(d);
    if (!head) {
        printf("cd ..\n");
    }
}

int
main(int argc, char **argv)
{
    traverse(argv[1], true);

    return 0;
}

