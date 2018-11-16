cimport woexp

__doc__ = 'Primitive wordexp.h wrapper'

cpdef enum:
    WRDE_DOOFFS = (1 << 0)	#  Insert PWORDEXP->we_offs NULLs.
    WRDE_APPEND = (1 << 1)	#  Append to results of a previous call.
    WRDE_NOCMD = (1 << 2)	#  Don't do command substitution.
    WRDE_REUSE = (1 << 3)	#  Reuse storage in PWORDEXP.
    WRDE_SHOWERR = (1 << 4)	#  Don't redirect stderr to /dev/null.
    WRDE_UNDEF = (1 << 5)	#  Error for expanding undefined variables.

cdef class WordExp:
    'wordexp.h wrapper class'
    cdef woexp.wordexp_t data

    def __init__(self, char *s, int flags=0):
        '''__init__(s, flags)\n\nPerform wordexp(s, self.data, flags)'''
        self.expand(s, flags&~WRDE_REUSE)

    def __dealloc__(self):
        woexp.wordfree(&self.data)

    def expand(self, char *s, int flags=WRDE_REUSE):
        '''expand(s, flags=WRDE_REUSE)\n\nPerform wordexp(s, self.data, flags)'''
        if woexp.wordexp(s, &self.data, flags|WRDE_REUSE):
            raise MemoryError("Cannot perform expansion")

    def result(self):
        '''result()\n\nReturn list of filenames.'''
        return [self.data.we_wordv[i+self.data.we_offs] for i in xrange(self.data.we_wordc)]
