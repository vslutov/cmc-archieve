#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int
main(int argc, char **argv)
{
    DIR *d = opendir(argv[1]);
    struct dirent *dd;
    long long int all_size = 0;

    while ((dd = readdir(d)) != NULL) {
        char buf[PATH_MAX];
        snprintf(buf, sizeof(buf), "%s/%s", argv[1], dd->d_name);

        struct stat stb;
        if (lstat(buf, &stb) < 0) { // Maybe file was deleted? [race condition]
            continue;
        }


        if (S_ISREG(stb.st_mode)) {
            all_size += stb.st_size;
        }
    }

    printf("%lld\n", all_size);

    closedir(d);
    return 0;
}
