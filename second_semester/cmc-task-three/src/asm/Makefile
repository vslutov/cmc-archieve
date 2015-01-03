NASM = nasm

NASM_FLAGS = -I $(INCLUDE_DIR)/asm/ -f elf -D SORT_KIND=$(SORT_KIND)

ASM_DIR = $(SOURCE_DIR)/asm
vpath %.asm $(ASM_DIR)

$(BUILD_DIR)/%.o : %.asm $(BUILD_DIR)
	$(NASM) $(NASM_FLAGS) -o $@ $<

FILES_ASM = $(basename $(notdir $(wildcard $(ASM_DIR)/*.asm)))

OBJECT_ASM = $(foreach FILE, $(FILES_ASM), $(BUILD_DIR)/$(FILE).o)

