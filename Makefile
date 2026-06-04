# Variables
CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -c
LD = ld
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

# Source Directories
BOOT_DIR = src/boot
KERNEL_DIR = src/kernel
LIB_DIR = src/lib
DRIVER_DIR = $(KERNEL_DIR)/drivers
CONSOLE_DIR = $(KERNEL_DIR)/console
CPU_DIR = $(KERNEL_DIR)/cpu

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
	$(BUILD_KERNEL)/idt.o \
	$(BUILD_KERNEL)/idt_load.o \
	$(BUILD_KERNEL)/isr.o \
	$(BUILD_KERNEL)/interrupts.o \
	$(BUILD_LIB)/string.o

# The default target
all: run

# Create Build Directories
dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_BOOT)
	mkdir -p $(BUILD_KERNEL)
	mkdir -p $(BUILD_LIB)

# Source Compilation
$(BUILD_DIR)/bootsect.bin: $(BOOT_DIR)/bootsect.asm | dirs
	nasm -f bin $< -o $@

$(BUILD_KERNEL)/kernel_entry.o: $(KERNEL_DIR)/kernel_entry.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/kernel.o: $(KERNEL_DIR)/kernel.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/vga.o: $(DRIVER_DIR)/vga.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/console.o: $(CONSOLE_DIR)/console.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/idt.o: $(CPU_DIR)/idt.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/idt_load.o: $(CPU_DIR)/idt_load.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/isr.o: $(CPU_DIR)/isr.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/interrupts.o: $(CPU_DIR)/interrupts.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_LIB)/string.o: $(LIB_DIR)/string.c | dirs
	$(CC) $(CFLAGS) $< -o $@


# Kernel Linking
$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

# OS Imae
$(BUILD_DIR)/os_image.bin: $(BUILD_DIR)/bootsect.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@
	dd if=/dev/zero bs=512 count=50 >> $@

# Run the OS in QEMU
run: $(BUILD_DIR)/os_image.bin
	qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/os_image.bin

# Clean up all the generated files so we can start fresh
clean:
	rm -rf $(BUILD_DIR)/