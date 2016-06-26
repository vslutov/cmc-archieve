CXX = g++
CXXFLAGS = -O3 -Wall -std=c++0x

# Strict compiler options
CXXFLAGS += -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers \
            -Winit-self -Wswitch-default -Wfloat-equal -Wshadow \
			-Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
			-Wmissing-field-initializers -Wctor-dtor-privacy  \
			-Wnon-virtual-dtor -Wstrict-null-sentinel \
			-Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ \
			-std=gnu++1y -march=native

# Directories with source code
SRC_DIR = src
INCLUDE_DIR = include

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
DEP_DIR = $(BUILD_DIR)/deps
TXT_DIR = $(BUILD_DIR)/txt

DATA_DIR = data
DATA_BINARY = $(DATA_DIR)/binary
DATA_MULTICLASS = $(DATA_DIR)/multiclass

# Bridge - place for main project to pull headers and compiled libraries.
# Headers must be placed into BRIDGE_LIBRARY_DIR before dependency generation
# proceed. Libraries in BRIDGE_LIBRARY_DIR will be required on linkage step.
BRIDGE_MAKE = bridge/Makefile
BRIDGE_INCLUDE_DIR = bridge/include
BRIDGE_LIBRARY_DIR = bridge/lib

# Which libraries to prepare for usage: targets must be defined in BRIDGE_MAKE.
BRIDGE_TARGETS = easybmp argvparser liblinear

# Link libraries gcc flag: library will be searched with prefix "lib".
LDFLAGS = -leasybmp -largvparser -llinear -lgtest

# Add headers dirs to gcc search path
CXXFLAGS += -I $(INCLUDE_DIR) -I $(BRIDGE_INCLUDE_DIR)
# Add path with compiled libraries to gcc search path
CXXFLAGS += -L $(BRIDGE_LIBRARY_DIR)

# Helper macros
# subst is sensitive to leading spaces in arguments.
make_path = $(addsuffix $(1), $(basename $(subst $(2), $(3), $(4))))
# Takes path list with source files and returns pathes to related objects.
src_to_obj = $(call make_path,.o, $(SRC_DIR), $(OBJ_DIR), $(1))
# Takes path list with object files and returns pathes to related dep. file.
# Dependency files will be generated with gcc -MM.
src_to_dep = $(call make_path,.d, $(SRC_DIR), $(DEP_DIR), $(1))
# Default target (make without specified target).
.DEFAULT_GOAL := all

# Alias to make all targets.
.PHONY: all
all: $(BIN_DIR)/task2

# Suppress makefile rebuilding.
Makefile: ;

# "Hack" to restrict order of actions:
# 1. Make bridge files target.
# 2. Generate and include dependency information.
ifneq ($(MAKECMDGOALS), clean)
-include bridge.touch
endif

# bridge.touch contains redirect to dependency generation Makefile.
bridge.touch: $(wildcard $(BRIDGE_INCLUDE_DIR)/*) \
		$(wildcard $(BRIDGE_LIBRARY_DIR)/*)
	mkdir -p $(BRIDGE_INCLUDE_DIR) $(BRIDGE_LIBRARY_DIR)
	make -C $(dir $(BRIDGE_MAKE)) -f $(notdir $(BRIDGE_MAKE)) $(BRIDGE_TARGETS)
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DEP_DIR)
	echo "include deps.mk" > $@

OBJFILES = $(OBJ_DIR)/classifier.o $(OBJ_DIR)/hog.o $(OBJ_DIR)/image.o

# Rules for compiling targets
$(BIN_DIR)/task2: $(OBJFILES) $(OBJ_DIR)/main.o bridge.touch
	$(CXX) $(CXXFLAGS) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(BIN_DIR)/test: $(OBJFILES) $(OBJ_DIR)/test.o bridge.touch
	$(CXX) $(CXXFLAGS) $(filter %.o, $^) -o $@ $(LDFLAGS)

# Pattern for generating dependency description files (*.d)
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -E -MM -MT $(call src_to_obj, $<) -MT $@ -MF $@ $<

# Pattern for compiling object files (*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $(call src_to_obj, $<) $<

$(TXT_DIR):
	mkdir -p $(TXT_DIR)

$(TXT_DIR)/binary_model.txt: $(BIN_DIR)/task2 $(DATA_BINARY)/train_labels.txt
	./$(BIN_DIR)/task2 -d $(DATA_BINARY)/train_labels.txt -m $(TXT_DIR)/binary_model.txt --train

$(TXT_DIR)/multiclass_model.txt: $(BIN_DIR)/task2 $(DATA_MULTICLASS)/train_labels.txt
	./$(BIN_DIR)/task2 -d $(DATA_MULTICLASS)/train_labels.txt -m $(TXT_DIR)/multiclass_model.txt --train

$(TXT_DIR)/binary_predict.txt: $(BIN_DIR)/task2 $(TXT_DIR) $(TXT_DIR)/binary_model.txt $(DATA_BINARY)/test_labels.txt
	cd $(TXT_DIR) && \
	../../$(BIN_DIR)/task2 -d ../../$(DATA_BINARY)/test_labels.txt -m binary_model.txt -l binary_predict.txt --predict

$(TXT_DIR)/multiclass_predict.txt: $(BIN_DIR)/task2 $(TXT_DIR) $(TXT_DIR)/multiclass_model.txt $(DATA_MULTICLASS)/test_labels.txt
	cd $(TXT_DIR) && \
	../../$(BIN_DIR)/task2 -d ../../$(DATA_MULTICLASS)/test_labels.txt -m multiclass_model.txt -l multiclass_predict.txt --predict

# Fictive target
.PHONY: clean test run doc

doc:
	doxygen Doxyfile

# Delete all temprorary and binary files
clean:
	make -C $(dir $(BRIDGE_MAKE)) -f $(notdir $(BRIDGE_MAKE)) clean
	rm -rf $(BUILD_DIR) $(BRIDGE_INCLUDE_DIR) $(BRIDGE_LIBRARY_DIR) doc
	rm -f bridge.touch

run: $(TXT_DIR)/binary_predict.txt $(TXT_DIR)/multiclass_predict.txt $(DATA_BINARY)/test_labels.txt $(DATA_MULTICLASS)/test_labels.txt
	./compare.py $(DATA_BINARY)/test_labels.txt $(TXT_DIR)/binary_predict.txt && \
	./compare.py $(DATA_MULTICLASS)/test_labels.txt $(TXT_DIR)/multiclass_predict.txt

test: $(BIN_DIR)/test
	./$(BIN_DIR)/test

# If you still have "WTF?!" feeling, try reading teaching book
# by Mashechkin & Co. http://unicorn.ejudge.ru/instr.pdf
