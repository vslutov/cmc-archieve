PROJECT=task01
EXEFILE=sequence

BUILD=$(EXEFILE) $(EXEFILE).o

CXXFLAGS = -std=c++11 -O3

$(EXEFILE): $(EXEFILE).o
	$(CXX) $(LDFLAGS) -o $@ $^

all : $(EXEFILE)

clean:
	rm -f $(BUILD)

test: $(EXEFILE)
	./$(EXEFILE)

.PHONY : all clean test
