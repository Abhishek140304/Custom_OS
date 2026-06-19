#include "heap.h"
#include "pmm.h"
#include "../console/console.h"

static unsigned int heap_start = 0;
static unsigned int heap_end = 0;
static unsigned int heap_limit = 0;

unsigned int get_heap_start(){
    return heap_start;
}

unsigned int get_heap_end(){
    return heap_end;
}

void* kmalloc(unsigned int size){
    if(size % 4 != 0){
        size += 4 - (size % 4);
    }

    while(heap_end + size > heap_limit){
        void* page = pmm_alloc_page();
        printk("\nHeap Page : ");
        printk_hex((unsigned int)page);
        printk("\n");

        if(page == 0){
            printk("\nkmalloc: Out of memory\n");
            return 0;
        }

        heap_limit += PAGE_SIZE;

        printk("\nHeap grew by one page");
        printk("\nNew Heap Limit : ");
        printk_hex(heap_limit);
        printk("\n");
    }

    unsigned int current = heap_end;

    heap_end += size;

    return (void*)current;
}

void heap_init(){
    heap_start = (pmm_bitmap_end() + 0xFFF) & 0xFFFFF000;
    heap_end = heap_start;
    heap_limit = heap_start;
}