#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char filepath[PATH_MAX + 1];

int
main (int argc, char **argv)
{
  char *homedir = getenv("HOME");
  if (homedir == NULL) {
    fprintf(stderr, "Enviroment variable $HOME is not set.\nCan't do helpfull work.");
    exit(1);
  }

  char *filename = strrchr(argv[0], '/');
  filename = filename != NULL ? filename + 1 : argv[0];

  snprintf(filepath, sizeof(filepath), "%s/.%s",  homedir, filename);

  FILE *conf_file = fopen(filepath, "w");
  if (conf_file == NULL) {
    fprintf(stderr, "Can't open config file to write.");
    exit(1);
  }

  fprintf(conf_file, "1.0");
  fclose(conf_file);

  return 0;
}

