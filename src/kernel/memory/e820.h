/*
Defines the structure for the BIOS E820 memory map. This allows our C kernel to safely read the raw array of memory chunks dumped by the bootloader at physical address 0x5000.
*/

#ifndef E820_H
#define E820_H

#define MEMORY_COUNT_ADDR 0x7FF0
#define MEMORY_MAP_ADDR 0x8000

// single 24-byte entry in the memory map
typedef struct {
    unsigned long long base;
    unsigned long long length;

    /*
    1 = Usable (Free RAM available for OS to use)
    2 = Reserved (Hardware/BIOS is using this, DO NOT TOUCH!)
    3 = ACPI Reclaimable memory
    4 = ACPI NVS memory
    5 = Bad Memory (Faulty RAM stick)
    */
    unsigned int type;
    unsigned int acpi;
} __attribute__((packed)) e820_entry_t;

void e820_print_map();
unsigned int e820_total_usable_ram();
unsigned int e820_total_reserved_ram();
unsigned int e820_largest_usable_region();

#endif