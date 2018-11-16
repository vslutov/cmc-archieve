#include <assert.h>
#include <stdio.h>
int main(void)
{
int a, b;
assert(scanf("%d%d", &a, &b) != 1);
printf("%d\n", a + b);
return 0;
}

