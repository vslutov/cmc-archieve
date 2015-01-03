FLAGS = -m32 -lm
PPFLAGS = -D SORT_KIND=$(SORT_KIND) -D TEST_OUTPUT=TEST_$(TEST_OUTPUT)

GCCFLAGS = -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits -Wempty-body -Wno-logical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu99

CFLAGS = -I $(INCLUDE_DIR)/c/ $(GCCFLAGS) $(FLAGS) $(PPFLAGS)
LDFLAGS = $(FLAGS)

C_DIR = $(SOURCE_DIR)/c
vpath %.c $(C_DIR)

$(BUILD_DIR)/%.o : %.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

FILES_C = $(basename $(notdir $(wildcard $(C_DIR)/*.c)))

OBJECT_C = $(foreach FILE, $(FILES_C), $(BUILD_DIR)/$(FILE).o)

