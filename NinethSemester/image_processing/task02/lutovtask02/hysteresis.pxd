import cython
cimport numpy as np

#cython: boundscheck=False
#cython: wraparound=False
#cython: initializedcheck=False
#cython: cdivision=True
#cython: nonecheck=False

cdef np.uint8_t[:, :] used
cdef np.int32_t[:, :] coords

@cython.locals(rows=cython.int,
               cols=cython.int,
               i=cython.int,
               j=cython.int,
               h_i=cython.int,
               h_j=cython.int,
               n_i=cython.int,
               n_j=cython.int,
               head=cython.int,
               tail=cython.int,
               step_i=cython.int,
               step_j=cython.int)
cdef np.ndarray[np.uint8_t, ndim=2] hysteresis(np.ndarray[np.uint8_t, ndim=2] result,
                                               np.uint8_t[:, :] mask)
