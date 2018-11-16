hello : hello.o calls.o start.o
	ld $^ -o $@

clean :
	rm -rf *.o hello

%.o : %.c
	gcc -s -static -c -Wall -o $@ $<

%.o : %.asm
	fasm $<
