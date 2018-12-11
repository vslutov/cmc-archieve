PROJECT=task01
SEQUENCE=sequence
PARMPI=parmpi
PARMPIMP=parmpimp
EXECUTABLE=$(SEQUENCE) $(PARMPI) $(PARMPIMP)

CXX = mpicxx
CXXFLAGS = -std=c++11 -O3 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel -Woverloaded-virtual -Wsign-promo -fopenmp -lpthread

%: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

all : $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

test: all
	mpirun -n 4 $(PARMPIMP)

.PHONY : all clean test
