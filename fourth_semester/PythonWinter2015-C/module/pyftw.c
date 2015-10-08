#include <Python.h>

#include <stdio.h>
#include <ftw.h>

static PyObject *values = NULL;
static PyObject *func = NULL;

static int
exec_elem(const char *fpath, const struct stat *s, int typeflag)
{
    PyObject *local_values = values,
             *local_func = func;

    const char *type;
    switch (typeflag) {
        case FTW_F:
            type = "File";
            break;
        case FTW_D:
            type = "Dir";
            break;
        case FTW_DNR:
            type = "DNR";
            break;
        case FTW_NS:
            type = "Not sumbolic";
            break;
        default:
            type = "Error";
            break;
    }

    PyObject *arglist = Py_BuildValue("(ss)", fpath, type);
    PyObject *result = PyObject_CallObject(local_func, arglist);
    Py_DECREF(arglist);

    values = local_values;
    func = local_func;

    if (result == NULL) {
        return 1;
    } else if (result != Py_None) {
        PyList_Append(values, result);
    }

    return 0;
};

static PyObject *
pyftw(PyObject *self, PyObject *args) {
    const char *dirpath;

    if (!PyArg_ParseTuple(args, "sO", &dirpath, &func)) {
        return NULL;
    } else {
        if (!PyCallable_Check(func)) {
            PyErr_SetString(PyExc_TypeError, "must be callable");
            return NULL;
        }

        Py_INCREF(func);
        values = PyList_New(0);
        int error = ftw(dirpath, exec_elem, 10);
        Py_DECREF(func);

        if (!error) {
            return values;
        } else {
            if (error == -1) {
                PyErr_SetString(PyExc_RuntimeError, "error in ftw library");
            }
            Py_DECREF(values);
            return NULL;
        }
    }
}

static PyMethodDef FtwMethods[] = {
    {"ftw",  pyftw, METH_VARARGS,
        "ftw(dirpath, func(fpath, ftype))\n\n"
        "Ftw wrapper.\n"
        "Return array of func results (which is not None)\n"
        "ftype is File, Dir, DNR or Not symbolic (see `man ftw`)\n"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initpyftw(void)
{
    PyObject *module;

    module = Py_InitModule3("pyftw", FtwMethods, "Ftw wrapper for python.");
    if(module == NULL) {
        return;
    }
}

int
main(int argc, char *argv[])
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    initpyftw();

    return 0;
}

