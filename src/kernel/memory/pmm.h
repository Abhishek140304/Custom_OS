#ifndef PMM_H
#define PMM_H

#define PMM_BITMAP ((unsigned char*)0x100000)

#define PAGE_SIZE 4096

unsigned int pmm_bitmap_start();
unsigned int pmm_bitmap_end();
void pmm_init();
void* pmm_alloc_page();
void pmm_free_page(void* page);

#endif