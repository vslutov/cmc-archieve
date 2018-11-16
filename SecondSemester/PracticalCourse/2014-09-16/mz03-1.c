#include <stdio.h>

int
main(int argc, char **argv)
{
    double account;
    sscanf(argv[1], "%lg", &account);

    double positive_multiplier;
    sscanf(argv[2], "%lg", &positive_multiplier);
    positive_multiplier = positive_multiplier * 0.01 + 1.0;

    double negative_multiplier;
    sscanf(argv[3], "%lg", &negative_multiplier);
    negative_multiplier = negative_multiplier * 0.01 + 1.0;

    for(int i = 4; argv[i] != NULL; ++i) {
        double check;
        sscanf(argv[i], "%lg", &check);

        account += check;

        if (account > 0) {
            account *= positive_multiplier;
        } else {
            account *= negative_multiplier;
        }
    }

    printf("%.10lg\n", account);

    return 0;
}
