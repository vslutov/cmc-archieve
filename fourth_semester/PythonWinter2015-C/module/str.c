#include <Python.h>

int main(int argc, char *argv[]) {
  PyObject *expr[3];
  int i, s, e, r;
  char *res;

  if(argc<5) {
    fprintf(stderr,"Usage: <string> <start> <end> <repeat>\n\n\
Print string[start:end]*repeat");
    exit(0);
  }
  s = atoi(argv[2]);
  e = atoi(argv[3]);
  r = atoi(argv[4]);
  expr[0] = PyString_FromString(argv[1]);
  expr[1] = PySequence_GetSlice(expr[0], s, e);
  expr[2] = PySequence_Repeat(expr[1], r);
  res=PyString_AsString(expr[2]);
  printf("'%s'\n",res);
  for(i=0; i<3; i++) Py_CLEAR(expr[i]);
  return 0;
}
