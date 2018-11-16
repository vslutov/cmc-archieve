#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
  size_t len = argc;
  for (size_t i = 1; i < argc; ++ i)
    len += strlen(argv[i]);

  char *buffer = calloc(len, sizeof (*buffer));
  assert(buffer != NULL);

  char *tail = buffer;

  for (size_t i = 1; i < argc; ++ i) {
    tail = strcpy(tail, argv[i]);
    tail += strlen(argv[i]);

    if (i != argc - 1) {
      *tail = ' ';
      ++ tail;
    }
  }

  printf("%s\n", buffer);

  free(buffer);

  return 0;
}

