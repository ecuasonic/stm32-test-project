CROSS		:= /usr/bin/arm-none-eabi
GCC			:= $(CROSS)-gcc
OBJCOPY		:= $(CROSS)-objcopy

SRC_DIR		:= src
INC_DIR		:= include
BUILD_DIR	:= build

SRC			:= $(wildcard $(SRC_DIR)/*.c)
OBJ			:= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
TARGET_ELF	:= $(BUILD_DIR)/firmware.elf
TARGET_BIN	:= firmware.bin

CFLAGS		:= -mcpu=cortex-m3 -mthumb -Os -ffreestanding
LDFLAGS		:= -T linker.ld -nostdlib

.PHONY: all
all: $(TARGET_BIN)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TARGET_ELF) : $(OBJ)
	$(GCC) $(LDFLAGS) $^ -o $@

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

.PHONY: flash 
flash: $(TARGET_BIN)
	st-flash --reset write $(TARGET_BIN) 0x8000000

.PHONY: debug
debug:
	$(MAKE) CFLAGS="$(CFLAGS) -g"

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET_BIN)
