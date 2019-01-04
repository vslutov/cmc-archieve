CXXFLAGS=-std=c++17 -O2 -Wformat -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -Wctor-dtor-privacy -Werror -pthread

all : part1 part2-main part2-test
.PHONY: all clean

part2-main : part2.o part2-main.o
	$(LINK.cc) -o $@ $^

part2-test : part2.o part2-test.o
	$(LINK.cc) -o $@ $^

clean :
	rm -rf part1 part2-main part2-test
