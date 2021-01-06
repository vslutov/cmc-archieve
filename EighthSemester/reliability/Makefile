# CFLAGS=-O2 -Wall -Werror -Wno-pointer-sign -std=gnu11
CFLAGS= -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign
MAIN=420_lutov

all: ${MAIN}

test: all
	./${MAIN} --lts 1 2 3 4

clean:
	rm -rf ${MAIN}

.PHONY: all test clean
