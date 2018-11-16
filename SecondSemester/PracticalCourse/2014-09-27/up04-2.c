#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

enum {
    MAX_GROUP = 32,
    MAX_LEN = 1022
};

bool
in_list(gid_t val, uid_t *list, size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        if (list[i] == val) {
            return true;
        }
    }

    return false;
}

int
main(void)
{
    uid_t p_uid;
    intmax_t input_uid;
    int all_right = scanf("%jd\n", &input_uid);
    assert(all_right == 1);
    p_uid = input_uid;

    char s_gids[MAX_LEN];
    gid_t p_gids[MAX_GROUP];
    size_t gid_count = 0;
    char *string = fgets(s_gids, MAX_LEN, stdin);
    assert(string != NULL);

    int r;
    intmax_t input_gid;
    while (sscanf(string, "%jd%n", &input_gid, &r) > 0) {
        string += r;
        p_gids[gid_count] = input_gid;
        ++ gid_count;
    }

    int op;
    all_right = scanf("%o\n", &op);
    assert(all_right == 1);

    uid_t uid;
    gid_t gid;
    int perms;
    while (scanf("%jd %jd %o", &input_uid, &input_gid, &perms) > 0) {
        uid = input_uid;
        gid = input_gid;

        char c = getchar();
        while (isspace(c)) {
            c = getchar();
        }

        char filename[MAX_LEN];
        filename[0] = c;
        string = fgets(filename + 1, MAX_LEN - 1, stdin);
        assert(string != NULL);
        int len = strlen(filename);
        if (filename[len - 1] == '\n') {
            filename[len - 1] = 0;
        }

        int user_perm;

        if (uid == p_uid) {
            user_perm = perms >> 6;
        } else if (in_list(gid, p_gids, gid_count)) {
            user_perm = (perms >> 3) & 7;
        } else {
            user_perm = perms & 7;
        }

        if ((user_perm & op) == op) {
            printf("%s\n", filename);
        }
    }

    return 0;
}

