PROJECT=task01
EXEFILE=task01

ALGLIB_OBJECT_FILES := $(patsubst %.cpp,%.o,$(wildcard alglib/*.cpp))

BUILD=$(EXEFILE) $(EXEFILE).o $(ALGLIB_OBJECT_FILES)

CXX = mpicxx
CXXFLAGS = -std=c++11 -fopenmp -O3

LDFLAGS = -lscalapack -llapack -lgomp

$(EXEFILE): $(EXEFILE).o $(ALGLIB_OBJECT_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

all : $(EXEFILE)

clean:
	rm -f $(BUILD)

test : all
	/bin/bash bsubscript

.PHONY : all clean test
