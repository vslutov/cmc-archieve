CXXFLAGS=-std=c++17 -g

all : main
.PHONY: all test clean

test : main
	valgrind ./main

clean :
	rm -rf main
