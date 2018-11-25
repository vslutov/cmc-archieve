BASIC_CXXFLAGS=-std=c++17 -O2 -Wformat -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -Wctor-dtor-privacy
CXXFLAGS=$(BASIC_CXXFLAGS) -Werror

EXEFILE = main

ALGLIB_OBJECT_FILES := $(patsubst %.cpp,%.o,$(wildcard alglib/*.cpp))

SOURCE_FILES := $(wildcard *.cpp)
OBJECT_FILES := $(SOURCE_FILES:%.cpp=%.o) $(ALGLIB_OBJECT_FILES)

alglib/%.o : alglib/%.cpp
	$(CXX) $(BASIC_CXXFLAGS) -c -o $@ $<

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all : $(EXEFILE)
.PHONY: all test clean

test : $(EXEFILE)
	./$(EXEFILE)

clean :
	rm -rf $(EXEFILE) $(OBJECT_FILES)

$(EXEFILE) : $(OBJECT_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^
