#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

enum {
  BUFFER_SIZE = 4096
};

char buffer[BUFFER_SIZE];

int
main(int argc, char **argv) {
  int return_value = 1;

  int fdw = 1;
  int fdr = -1;

  for (size_t i = 1; argv[i] != NULL; ++i) {
    fdr = open(argv[i], O_RDONLY);

    if (fdr < 0) {
      fprintf(stderr, "Error opening file `%s`.\n", argv[i]);
      goto cleanup;
    }

    int r;
    while ((r = read(fdr, buffer, sizeof(buffer))) > 0) {
      char *w = buffer;
      int t = 0;

      while (r > 0 && (t = write(fdw, w, r)) > 0) {
        r -= t;
        w += t;
      }

      if (r < 0) {
        fprintf(stderr, "Error writting to stdout");
        goto cleanup;
      }
    }

    if (r < 0) {
      fprintf(stderr, "Error reading from file `%s`\n", argv[i]);
      goto cleanup;
    }
  }

  return_value = 0;

cleanup:
  if (fdw >= 0) {
    close(fdw);
  }

  if (fdr >= 0) {
    close(fdr);
  }

  return return_value;
}


