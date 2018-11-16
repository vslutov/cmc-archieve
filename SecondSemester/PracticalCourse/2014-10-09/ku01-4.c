#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

enum {
    INIT_SIZE = 300,
    SIZE_MULTIPLIER = 6
};

struct File
{
    intmax_t size;
    char name[PATH_MAX];
};

bool
get_file(const char *foldername, DIR *folder, struct File *file)
{
    struct dirent *file_dirent;
    while ((file_dirent = readdir(folder)) != NULL) {
        if (strcmp(file_dirent->d_name, "..") == 0 ||
            strcmp(file_dirent->d_name, ".") == 0) {
            continue;
        }

        char filepath[PATH_MAX];
        snprintf(filepath, PATH_MAX, "%s/%s", foldername, file_dirent->d_name);

        struct stat fstat;
        if (lstat(filepath, &fstat) < 0 || !S_ISREG(fstat.st_mode)) {
            continue;
        } else {
            snprintf(file->name, PATH_MAX, "%s", file_dirent->d_name);
            file->size = fstat.st_size;
            return true;
        }
    }

    return false;
}

bool
has_suffix(const char *string, const char *suffix)
{
    size_t string_length = strlen(string);
    size_t suffix_length = strlen(suffix);

    if (string_length < suffix_length) {
        return false;
    } else {
        if (strcmp(string + string_length - suffix_length, suffix) == 0) {
            return true;
        } else {
            return false;
        }
    }
}

int
main(int argc, char **argv)
{
    char *foldername = argv[1];
    DIR *folder = opendir(foldername);
    assert(folder != NULL);

    intmax_t max_size;
    sscanf(argv[2], "%jd", &max_size);

    const char *suffix = argv[3];

    size_t real_size = INIT_SIZE;
    size_t virtual_size = 0;

    struct File *file_array = malloc(sizeof(*file_array) * real_size);
    assert(file_array != NULL);

    intmax_t size_sum = 0;

    struct File file;
    while (get_file(foldername, folder, &file)) {
        size_sum += file.size;

        if (has_suffix(file.name, suffix)) {
            if (virtual_size != 0) {
                if (file_array[0].size < file.size) {
                    virtual_size = 0;
                } else if (file_array[0].size > file.size) {
                    continue;
                }
            }

            if (virtual_size == real_size) {
                real_size *= SIZE_MULTIPLIER;
                file_array = realloc(
                    file_array,
                    sizeof(*file_array) * real_size);
                assert(file_array != NULL);
            }

            file_array[virtual_size] = file;
            ++virtual_size;
        }
    }

    size_t min_num = 0;
    for (size_t i = 1; i < virtual_size; ++i) {
        if (strcmp(file_array[i].name, file_array[min_num].name) < 0) {
            min_num = i;
        }
    }

    if (size_sum > max_size && virtual_size > 0) {
        printf("%s\n", file_array[min_num].name);
    }

    free(file_array);

    return 0;
}

