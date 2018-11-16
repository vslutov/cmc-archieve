#include <Python.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc==1) {
    fprintf(stderr,"Usage: list <list of strings>\n\n"
                   "Return sorted list\n");
    exit(0);
  }

  Py_SetProgramName(argv[0]);
  Py_Initialize();

  PyObject *list =  PyList_New(0);
  for (ssize_t i = 1; i < argc; ++i) {
    PyObject *str = PyString_FromString(argv[i]);
    PyList_Append(list, str);
    Py_CLEAR(str);
  }

  PyList_Sort(list);
  for (ssize_t i = 0; i < PyList_Size(list); ++i) {
    printf("%s ", PyString_AsString(PyList_GetItem(list, i)));
  }
  printf("\n");

  Py_CLEAR(list);

  return 0;
}
