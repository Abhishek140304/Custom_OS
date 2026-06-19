#include "heap.h"
#include "pmm.h"

static unsigned int heap_start = 0;
static unsigned int heap_end = 0;

unsigned int get_heap_start(){
    return heap_start;
}

unsigned int get_heap_end(){
    return heap_end;
}

void heap_init(){
    heap_start = (pmm_bitmap_end() + 0xFFF) & 0xFFFFF000;

    heap_end = heap_start;
}