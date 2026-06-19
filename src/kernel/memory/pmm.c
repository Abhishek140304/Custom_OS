/*
Description: The Physical Memory Manager. It tracks the state of every 4KB frame of RAM in the system using a massive bit array (Bitmap), allowing the kernel to find and claim free memory safely.
*/


#include "pmm.h"
#include "e820.h"

// Linker symbols to tell us where our kernel physically lives in RAM
extern char kernel_start;
extern char kernel_end;

// Global State Variables
static unsigned int bitmap_start = 0;
static unsigned int bitmap_size = 0;
static unsigned int total_pages = 0;
static unsigned int free_pages = 0;
static unsigned char* bitmap = 0;

// Marks a page as FREE (Sets its bit to 0)
static void bitmap_clear(unsigned int page){
    bitmap[page / 8] &= ~(1 << (page % 8));
}

// Marks a page as USED/RESERVED (Sets its bit to 1)
static void bitmap_set(unsigned int page){
    bitmap[page / 8] |= (1 << (page % 8));
}

// Checks if a page is USED (Returns > 0 if used, 0 if free)
static int bitmap_test(unsigned int page){
    return bitmap[page / 8] & (1 << (page % 8));
}

// Scans the BIOS memory map and sets Type 1 (Usable) RAM to FREE (0)
static void pmm_mark_usable_pages(){
    unsigned short count = *(unsigned short*)MEMORY_COUNT_ADDR;
    e820_entry_t* map = (e820_entry_t*)MEMORY_MAP_ADDR;

    for(int i=0; i<count; i++){
        if(map[i].type != 1) continue;

        // Convert the physical byte addresses into 4KB Page Indexes
        unsigned int start_page = (unsigned int)(map[i].base / PAGE_SIZE);
        unsigned int page_count = (unsigned int)(map[i].length / PAGE_SIZE);
        unsigned int end_page = start_page + page_count;

        for(unsigned int p=start_page; p<end_page; p++){
            bitmap_clear(p);
            free_pages++;
        }
    }
}

unsigned int pmm_bitmap_start(){
    return bitmap_start;
}

unsigned int pmm_bitmap_end(){
    return bitmap_start + bitmap_size;
}

// Finds the first available FREE page (bit = 0), marks it USED, and returns it.
void* pmm_alloc_page(){
    for(unsigned int page=0; page<total_pages; page++){
        if(bitmap_test(page) == 0){
            bitmap_set(page);

            free_pages--;

            return (void*)(page * PAGE_SIZE);
        }
    }

    return 0;
}

// Marks a previously used page as FREE again.
void pmm_free_page(void* addr){
    unsigned int page = ((unsigned int)addr) / PAGE_SIZE;

    bitmap_clear(page);

    free_pages++;
}

void pmm_reserve_page(unsigned int page){
    if(bitmap_test(page) == 0){
        bitmap_set(page);
        free_pages--;
    }
}

void pmm_reserve_region(unsigned int start, unsigned int end){
    unsigned int start_page = start / PAGE_SIZE;
    unsigned int end_page = (end + PAGE_SIZE - 1) / PAGE_SIZE;

    for(unsigned int page=start_page; page<end_page; page++){
        pmm_reserve_page(page);
    }
}

void pmm_init(){
    unsigned int usable_ram = e820_total_usable_ram();

    total_pages = usable_ram / PAGE_SIZE;

    // Place the bitmap safely in RAM immediately after the kernel ends, snapped cleanly to the next 4KB boundary.
    bitmap_start = ((unsigned int)&kernel_end + 0xFFF) & 0xFFFFF000;
    bitmap_size = (total_pages + 7) / 8;

    bitmap = (unsigned char*)bitmap_start;

    // Assume EVERYTHING is reserved. Set every bit to 1 (0xFF = 11111111)
    for(unsigned int i = 0; i < bitmap_size; i++){
        bitmap[i] = 0xFF;
    }

    // Look at the E820 map and ONLY clear the bits (to 0) for RAM that the motherboard specifically told us was safe to use
    pmm_mark_usable_pages();

    // Protect the bottom 1MB (BIOS, VGA Text Buffer, Bootloader)
    pmm_reserve_region(0x00000000, 0x00100000);

    // Protect the physical C Kernel
    pmm_reserve_region((unsigned int)&kernel_start, (unsigned int)&kernel_end);

    // Protect the Bitmap
    pmm_reserve_region(bitmap_start, bitmap_start+bitmap_size);
}