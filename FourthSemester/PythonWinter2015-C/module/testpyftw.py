import pyftw

def a(x, t):
    print x, t

pyftw.ftw('./', a)
