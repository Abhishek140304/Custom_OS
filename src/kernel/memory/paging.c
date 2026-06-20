/*
Description: Implements x86 32-bit Virtual Memory. It builds the Page Directory and Page Tables, loads them into the CPU, and activates the Memory Management Unit (MMU).
*/

#include "paging.h"
#include "pmm.h"
#include "page_fault.h"

typedef unsigned int page_dir_entry_t;
typedef unsigned int page_table_entry_t;

// The CPU REQUIRES that the Page Directory and Page Tables are perfectly aligned to 4096-byte (4KB) boundaries.
page_dir_entry_t kernel_directory[1024] __attribute__((aligned(4096)));
page_table_entry_t first_page_table[1024] __attribute__((aligned(4096)));

// Performs "Identity Mapping" for the first 4MB of RAM.
static void paging_setup_first_table(){
    for(unsigned int i = 0; i < 1024; i++){
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }
}

static void paging_setup_directory(){
    for(unsigned int i = 0; i < 1024; i++){
        kernel_directory[i] = 0;
    }

    kernel_directory[0] = ((unsigned int)first_page_table) | PAGE_PRESENT | PAGE_WRITABLE;
}

// Tells the CPU hardware where to find our Page Directory.
static void load_page_directory(unsigned int* directory){

    // CR3 is a special Control Register inside the CPU. Its ONLY job is to hold the physical memory address of the active Page Directory.
    asm volatile(
        "mov %0, %%cr3"
        :
        : "r"(directory) 
    );
}

// Flips the master switch to activate the MMU.
static void enable_paging(){
    unsigned int cr0;

    asm volatile(
        "mov %%cr0, %0"
        : "=r"(cr0)
    );

    // Bitwise OR with 0x80000000. This sets the 31st bit (the highest bit) of CR0 to 1. In x86 architecture, this is the "Paging Enable" (PG) flag.
    cr0 |= 0x80000000;

    asm volatile(
        "mov %0, %%cr0"
        :
        : "r"(cr0)
    );
}

static unsigned int paging_get_physical(unsigned int virtual_addr){
    
    unsigned int dir_index = virtual_addr >> 22;
    unsigned int table_index = (virtual_addr >> 12) & 0x3FF;

    page_dir_entry_t dir_entry = kernel_directory[dir_index];

    if(!(dir_entry & PAGE_PRESENT)) return 0;

    page_table_entry_t* table = (page_table_entry_t*) (dir_entry & 0xFFFFF000);

    page_table_entry_t table_entry = table[table_index];

    if(!(table_entry & PAGE_PRESENT)) return 0;

    return (table_entry & 0xFFFFF000) | (virtual_addr & 0xFFF);
}

unsigned int* paging_get_table(unsigned int virtual_addr, int create){
    unsigned int dir_index = virtual_addr >> 22;

    page_dir_entry_t entry = kernel_directory[dir_index];

    if(entry & PAGE_PRESENT){
        return (unsigned int*) (entry & 0xFFFFF000);
    }

    if(!create) return 0;

    unsigned int* table = (unsigned int*)pmm_alloc_page();

    if(table == 0) return 0;

    for(unsigned int i=0; i<1024; i++){
        table[i] = 0;
    }

    kernel_directory[dir_index] = ((unsigned int)table) | PAGE_PRESENT | PAGE_WRITABLE;

    return table;
}

void paging_map_page(unsigned int virtual_addr, unsigned int physical_addr, unsigned int flags){
    unsigned int table_index = (virtual_addr >> 12) & 0x3FF;

    unsigned int* table = paging_get_table(virtual_addr, 1);

    if(table == 0) return;

    table[table_index] = (physical_addr & 0xFFFFF000) | flags | PAGE_PRESENT;

    // THE TLB FLUSH (Translation Lookaside Buffer). The CPU caches memory translations to run faster. If we change a mapping, we MUST tell the CPU to delete its cached version, or it will keep using the old, incorrect mapping. 'invlpg' = Invalidate Page.
    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_addr)
        : "memory"
    );
}

void paging_unmap_page(unsigned int virtual_addr){
    unsigned int table_index = (virtual_addr >> 12) & 0x3FF;

    unsigned int* table = paging_get_table(virtual_addr, 0);

    if(!table) return;

    table[table_index] = 0;

    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_addr)
        : "memory"
    );
}

void paging_init(){
    paging_setup_first_table();
    paging_setup_directory();

    load_page_directory(kernel_directory);
    enable_paging();

    page_fault_init();

}