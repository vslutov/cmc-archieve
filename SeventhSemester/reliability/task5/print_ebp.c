#include <stdio.h>

int
get_ebp(void);

void
overflow(const char *arr)
{
  printf("%x\n", get_ebp());
}

int
main(int argc, const char **argv, const char **envp)
{
  overflow(argv[1]);
  return 0;
}
