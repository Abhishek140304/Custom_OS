#include "pmm.h"
#include "../console/console.h"
#include "e280.h"

extern char kernel_end;
static unsigned int bitmap_start = 0;
static unsigned int bitmap_size = 0;
static unsigned int total_pages = 0;
static unsigned int free_pages = 0;
static unsigned char* bitmap = 0;

static void bitmap_clear(unsigned int page){
    bitmap[page / 8] &= ~(1 << (page % 8));
}

static void bitmap_set(unsigned int page){
    bitmap[page / 8] |= (1 << (page % 8));
}

static int bitmap_test(unsigned int page){
    return bitmap[page / 8] & (1 << (page % 8));
}

static void pmm_mark_usable_pages(){
    unsigned short count = *(unsigned short*)MEMORY_COUNT_ADDR;
    e820_entry_t* map = (e820_entry_t*)MEMORY_MAP_ADDR;

    for(int i=0; i<count; i++){
        if(map[i].type != 1) continue;


        unsigned int start_page = (unsigned int)(map[i].base / PAGE_SIZE);
        unsigned int page_count = (unsigned int)(map[i].length / PAGE_SIZE);
        unsigned int end_page = start_page + page_count;

        if(end_page <= 256) continue;
        if(start_page < 256) start_page = 256;

        for(unsigned int p=start_page; p<end_page; p++){
            bitmap_clear(p);
            free_pages++;
        }
    }
}

void pmm_init(){
    unsigned int usable_ram = e820_total_usable_ram();

    total_pages = usable_ram / PAGE_SIZE;

    bitmap_start = ((unsigned int)&kernel_end + 0xFFF) & 0xFFFFF000;
    bitmap_size = (total_pages + 7) / 8;

    bitmap = (unsigned char*)bitmap_start;

    for(unsigned int i = 0; i < bitmap_size; i++){
        bitmap[i] = 0xFF;
    }

    pmm_mark_usable_pages();
}

unsigned int pmm_bitmap_start(){
    return bitmap_start;
}

unsigned int pmm_bitmap_end(){
    return bitmap_start + bitmap_size;
}

void* pmm_alloc_page(){
    if(free_pages < 1){
        printk("\nCannot Allocate as memory is full \n");
    }

    for(unsigned int page=0; page<total_pages; page++){
        if(bitmap_test(page) == 0){
            bitmap_set(page);

            free_pages--;

            return (void*)(page * PAGE_SIZE);
        }
    }

    return 0;
}

void pmm_free_page(void* addr){
    unsigned int page = ((unsigned int)addr) / PAGE_SIZE;

    bitmap_clear(page);

    free_pages++;
}
