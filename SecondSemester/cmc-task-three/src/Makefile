TARGET = $(BUILD_DIR)/$(PROJECT_NAME)
INCLUDE_DIR = $(SOURCE_DIR)/include

$(BUILD_DIR) :
	mkdir -p $(BUILD_DIR)

include $(SOURCE_DIR)/c/Makefile
include $(SOURCE_DIR)/asm/Makefile

$(TARGET) : $(OBJECT_ASM) $(OBJECT_C)
	$(CC) $(OBJECT_ASM) $(OBJECT_C) -o $(TARGET) $(LDFLAGS)

