#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>

enum {
    MAX_GROUP = 32,
    MAX_GROUP_LEN = 16 * 32
};

int
main(void)
{
    long long p_uid;
    int all_right = scanf("%lld\n", &p_uid);
    assert(all_right == 1);

    char s_gids[MAX_GROUP_LEN];
    long long int p_gids[MAX_GROUP];
    size_t gid_count = 0;
    char *string = fgets(s_gids, MAX_GROUP_LEN, stdin);
    assert(string != NULL);

    int r;
    while (sscanf(string, "%lld%n", &p_gids[gid_count], &r) > 0) {
        string += r;
        ++ gid_count;
    }

    int op;
    all_right = scanf("%o\n", &op);
    assert(all_right == 1);

    long long uid, gid;
    int perms;
    while (scanf("%lld %lld %o", &uid, &gid, &perms) > 0) {
        char c = getchar();
        while (isspace(c)) {
            c = getchar();
        }

        char filename[PATH_MAX + 2];
        filename [0] = c;
        string = fgets(filename + 1, PATH_MAX + 1, stdin);
        assert(string != NULL);
        int len = strlen(filename);
        if (filename[len - 1] == '\n') {
            filename[len - 1] = 0;
        }

        bool have_perm = false;

        if (uid == p_uid && (((perms & (7 << 6)) >> 6) & op) == op) {
            have_perm = true;
        }

        for (size_t i = 0; i < gid_count; ++i) {
            if (gid == p_gids[i]) {
                if ((((perms & (7 << 3)) >> 3) & op) == op) {
                    have_perm = true;
                }
            }
        }

        if ((perms & op) == op) {
            have_perm = true;
        }

        if (have_perm) {
            printf("%s\n", filename);
        }
    }

    return 0;
}
