# Variables
CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -c
LD = ld
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

# Source Directories
BOOT_DIR = src/boot
KERNEL_DIR = src/kernel
LIB_DIR = src/lib

# Build Directories
BUILD_DIR = build
BUILD_BOOT = $(BUILD_DIR)/boot
BUILD_KERNEL = $(BUILD_DIR)/kernel
BUILD_LIB = $(BUILD_DIR)/lib

# Object files
OBJS = \
	$(BUILD_KERNEL)/kernel_entry.o \
	$(BUILD_KERNEL)/kernel.o \
	$(BUILD_KERNEL)/vga.o \
	$(BUILD_KERNEL)/console.o \
	$(BUILD_LIB)/string.o

# The default target
all: run

# Create Build Directories
dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_BOOT)
	mkdir -p $(BUILD_KERNEL)
	mkdir -p $(BUILD_LIB)

# C Source Compilation
$(BUILD_KERNEL)/kernel.o: $(KERNEL_DIR)/kernel.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/vga.o: $(KERNEL_DIR)/vga.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/console.o: $(KERNEL_DIR)/console.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_LIB)/string.o: $(LIB_DIR)/string.c | dirs
	$(CC) $(CFLAGS) $< -o $@

# Assembly Compilation
$(BUILD_KERNEL)/kernel_entry.o: $(KERNEL_DIR)/kernel_entry.asm | dirs
	nasm -f elf $< -o $@

# Kernel Linking
$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

# Bootloader
$(BUILD_DIR)/bootsect.bin: $(BOOT_DIR)/bootsect.asm | dirs
	nasm -f bin $< -o $@

# OS Imae
$(BUILD_DIR)/os_image.bin: $(BUILD_DIR)/bootsect.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

# Run the OS in QEMU
run: $(BUILD_DIR)/os_image.bin
	qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/os_image.bin

# Clean up all the generated files so we can start fresh
clean:
	rm -rf $(BUILD_DIR)/