#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void
traverse(const char *p)
{
    DIR *d = opendir(p);
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
            traverse(buf);
        } else if (S_ISREG(stb.st_mode) && stb.st_size > 1000) {
            printf("%s\n", buf);
        }
    }

    closedir(d);
}

int
main(int argc, char **argv)
{
    traverse(argv[1]);

    return 0;
}

