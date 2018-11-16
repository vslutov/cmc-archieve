CFLAGS+=-I/usr/include/python2.7 --std=c99 -g -O0 -Wall
LDLIBS+=-lpython2.7

GENERATED=list woexp.c woexp.so pyftw.so pyftw.c

all: $(GENERATED)

%.c:	%.pyx
	cython $< -o $@

%.so: %.c
	$(CC) $< $(CFLAGS) -o $@ -shared -fPIC $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(GENERATED) *.so *.o

demo:	all
	./list list another list abacaba
	python testwoexp.py

test:	all
	valgrind ./list list another list abacaba
	valgrind python testwoexp.py
	valgrind python testpyftw.py
