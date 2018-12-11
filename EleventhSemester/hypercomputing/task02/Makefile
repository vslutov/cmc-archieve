PROJECT=task01
SEQUENCE=sequence
PARMPI=parmpi
EXECUTABLE=$(SEQUENCE) $(PARMPI)

BUILD=$(EXECUTABLE) $(SEQUENCE).o $(PARMPI).o

CXX = mpicxx
CXXFLAGS = -g -std=c++11 -O3 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel -Woverloaded-virtual -Wsign-promo

%: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

all : $(SEQUENCE) $(PARMPI)

clean:
	rm -f $(BUILD)

test: all
	mpirun -n 8 $(PARMPI)
	# ./$(SEQUENCE)

.PHONY : all clean test
