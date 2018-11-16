#include <stdlib.h>
#include <string.h>

enum {
    PERM_LENGTH = 9,
    GROUP_LENGTH = 3
};

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    if (strlen(str) != PERM_LENGTH) {
        return -1;
    }

    int result = 0;

    for (size_t i = 0; i < PERM_LENGTH; ++ i) {
        if (!(str[i] == '-' || str[i] == "rwx"[i % GROUP_LENGTH])) {
            return -1;
        }

        result <<= 1;

        if (str[i] != '-') {
            result |= 1;
        }
    }

    return result;
}

