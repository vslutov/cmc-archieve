CXX ?= g++
CC ?= gcc
CFLAGS = -Wall -Wconversion -O3 -fPIC
LIBS = blas/blas.a
#SHVER = 1
OS = $(shell uname)
#LIBS = -lblas

TARGET_DIR = lib

TARGET = liblinear.a
.DEFAULT_GOAL := $(TARGET)

# Compile static library
$(TARGET): linear.o tron.o blas/blas.a
	ar x blas/blas.a
	ar rcs $@ *.o

libtemp.a: linear.o tron.o
	ar rcs $@ linear.o tron.o

liblinear.a: linear.o tron.o blas/blas.a
	ar x blas/blas.a
	ar rcs $@ *.o

train: train.c liblinear.a
	$(CXX) $(CFLAGS) -o train train.c liblinear.a

predict: tron.o linear.o predict.c blas/blas.a
	$(CXX) $(CFLAGS) -o predict predict.c tron.o linear.o $(LIBS)

tron.o: tron.cpp tron.h
	$(CXX) $(CFLAGS) -c -o tron.o tron.cpp

linear.o: linear.cpp linear.h
	$(CXX) $(CFLAGS) -c -o linear.o linear.cpp

blas/blas.a: blas/*.c blas/*.h
	make -C blas OPTFLAGS='$(CFLAGS)' CC='$(CC)';

clean:
	make -C blas clean
	rm -f *~ *.o train predict liblinear.a
