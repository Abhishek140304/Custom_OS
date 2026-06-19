#ifndef PMM_H
#define PMM_H

// The x86 architecture standard for a memory page is exactly 4096 bytes (4KB).
#define PAGE_SIZE 4096

unsigned int pmm_bitmap_start();
unsigned int pmm_bitmap_end();
void pmm_init();
void* pmm_alloc_page();
void pmm_free_page(void* page);
void pmm_reserve_page(unsigned int page);
void pmm_reserve_region(unsigned int start, unsigned int end);

#endif