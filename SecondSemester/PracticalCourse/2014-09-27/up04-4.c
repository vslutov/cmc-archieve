#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum {
    START_SIZE = 16,
    SIZE_MULTIPLIER = 2
};

typedef char String[PATH_MAX];

int
comp(const void *a, const void *b)
{
    return strcasecmp(*(String *)a, *(String *)b);
}

void
rec(char *foldername)
{
    DIR *dir = opendir(foldername);
    if (dir != NULL) {
        size_t real_size = START_SIZE;
        size_t virtual_size = 0;
        String *files = malloc(sizeof(*files) * real_size);

        struct dirent *file;

        while ((file = readdir(dir)) != NULL) {
            if (strcmp(file->d_name, ".") == 0 ||
                strcmp(file->d_name, "..") == 0) {
                continue;
            }

            String filename;
            snprintf(
                filename,
                sizeof(filename),
                "%s/%s",
                foldername,
                file->d_name);

            if (access(filename, R_OK) != 0) {
                continue;
            }

            struct stat stf;
            if (lstat(filename, &stf) < 0) {
                continue;
            }

            if (S_ISDIR(stf.st_mode)) {

                memcpy(
                    files[virtual_size],
                    filename,
                    sizeof(files[virtual_size]));

                ++virtual_size;
                if (virtual_size == real_size) {
                    real_size *= SIZE_MULTIPLIER;
                    files = realloc(files, sizeof(*files) * real_size);
                }
            }
        }

        closedir(dir);

        qsort(files, virtual_size, sizeof(*files), comp);

        for (size_t i = 0; i < virtual_size; ++i) {
            char *sub_folder = strrchr(files[i], '/');
            printf("cd %s\n", sub_folder + 1);
            rec(files[i]);
            printf("cd ..\n");
        }

        free(files);
    }
}

int
main(int argc, char **argv)
{
    if (access(argv[1], R_OK) == 0) {
        rec(argv[1]);
    }

    return 0;
}
