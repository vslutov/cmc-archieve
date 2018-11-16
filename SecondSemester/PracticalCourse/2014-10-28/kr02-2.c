#include <stdio.h>

#include <time.h>

int
main(int argc, char **argv)
{
    int year, mday, wday;
    sscanf(argv[1], "%d", &year);
    sscanf(argv[2], "%d", &mday);
    sscanf(argv[3], "%d", &wday);
    wday %= 7;

    struct tm time;

    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 12;
    time.tm_mday = mday;
    time.tm_year = year - 1900;
    time.tm_isdst = -1;

    int count = 0;

    for (int mon = 0; mon < 12; ++mon) {
        time.tm_mon = mon;
        mktime(&time);
        if (time.tm_wday == wday) {
            ++count;
        }
    }

    printf("%d\n", count);

    return 0;
}
