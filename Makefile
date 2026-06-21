# Variables
CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -c
LD = ld
LDFLAGS = -m elf_i386 -T linker.ld

# Source Directories
BOOT_DIR = src/boot
KERNEL_DIR = src/kernel
LIB_DIR = src/lib
DRIVER_DIR = $(KERNEL_DIR)/drivers
CONSOLE_DIR = $(KERNEL_DIR)/console
CPU_DIR = $(KERNEL_DIR)/cpu
EXCEPTIONS_DIR = $(CPU_DIR)/exceptions
IDT_DIR = $(CPU_DIR)/idt
INTERRUPT_MANAGER_DIR = $(CPU_DIR)/interrupt_manager
IO_DIR = $(CPU_DIR)/io
IRQ_DIR = $(CPU_DIR)/irq
SHELL_DIR = $(KERNEL_DIR)/shell
MEMORY_DIR = $(KERNEL_DIR)/memory


# Build Directories
BUILD_DIR = build
BUILD_BOOT = $(BUILD_DIR)/boot
BUILD_KERNEL = $(BUILD_DIR)/kernel
BUILD_LIB = $(BUILD_DIR)/lib

# Object files
OBJS = \
	$(BUILD_KERNEL)/kernel_entry_asm.o \
	$(BUILD_KERNEL)/kernel.o \
	$(BUILD_KERNEL)/vga.o \
	$(BUILD_KERNEL)/timer.o \
	$(BUILD_KERNEL)/keyboard.o \
	$(BUILD_KERNEL)/console.o \
	$(BUILD_KERNEL)/interrupts.o \
	$(BUILD_KERNEL)/isr_asm.o \
	$(BUILD_KERNEL)/idt_load_asm.o \
	$(BUILD_KERNEL)/idt.o \
	$(BUILD_KERNEL)/interrupt_manager.o \
	$(BUILD_KERNEL)/ports.o \
	$(BUILD_KERNEL)/irq_asm.o \
	$(BUILD_KERNEL)/irq.o \
	$(BUILD_KERNEL)/pic.o \
	$(BUILD_KERNEL)/shell.o \
	$(BUILD_KERNEL)/commands.o \
	$(BUILD_KERNEL)/memory_info.o \
	$(BUILD_KERNEL)/e820.o \
	$(BUILD_KERNEL)/pmm.o \
	$(BUILD_KERNEL)/heap.o \
	$(BUILD_KERNEL)/paging.o \
	$(BUILD_KERNEL)/page_fault.o \
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

$(BUILD_KERNEL)/kernel_entry_asm.o: $(KERNEL_DIR)/kernel_entry.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/kernel.o: $(KERNEL_DIR)/kernel.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/vga.o: $(DRIVER_DIR)/vga.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/timer.o: $(DRIVER_DIR)/timer.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/keyboard.o: $(DRIVER_DIR)/keyboard.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/console.o: $(CONSOLE_DIR)/console.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/interrupts.o: $(EXCEPTIONS_DIR)/interrupts.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/isr_asm.o: $(EXCEPTIONS_DIR)/isr.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/idt_load_asm.o: $(IDT_DIR)/idt_load.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/idt.o: $(IDT_DIR)/idt.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/interrupt_manager.o: $(INTERRUPT_MANAGER_DIR)/interrupt_manager.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/ports.o: $(IO_DIR)/ports.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/irq_asm.o: $(IRQ_DIR)/irq.asm | dirs
	nasm -f elf $< -o $@

$(BUILD_KERNEL)/irq.o: $(IRQ_DIR)/irq.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/pic.o: $(IRQ_DIR)/pic.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/shell.o: $(SHELL_DIR)/shell.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/commands.o: $(SHELL_DIR)/commands.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/memory_info.o: $(MEMORY_DIR)/memory_info.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/e820.o: $(MEMORY_DIR)/e820.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/pmm.o: $(MEMORY_DIR)/pmm.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/heap.o: $(MEMORY_DIR)/heap.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/paging.o: $(MEMORY_DIR)/paging.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/page_fault.o: $(MEMORY_DIR)/page_fault.c | dirs
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_LIB)/string.o: $(LIB_DIR)/string.c | dirs
	$(CC) $(CFLAGS) $< -o $@


# Kernel Linking
$(BUILD_DIR)/kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	objcopy -O binary $< $@

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