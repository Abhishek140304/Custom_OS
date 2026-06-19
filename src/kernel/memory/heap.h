#ifndef HEAP_H
#define HEAP_H

unsigned int get_heap_start();
unsigned int get_heap_end();
void heap_init();
void* kmalloc(unsigned int size);

#endif