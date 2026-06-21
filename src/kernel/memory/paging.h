#ifndef PAGING_H
#define PAGING_H

#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2
#define PAGE_USER 0x4

void paging_init();
unsigned int* paging_get_table(unsigned int virtual_addr, int create);
void paging_map_page( unsigned int virtual_addr, unsigned int physical_addr, unsigned int flags);
void paging_unmap_page(unsigned int virtual_addr);
void paging_map_range(unsigned int virt, unsigned int phys, unsigned int size, unsigned int flags);
void paging_map_kernel_high();

#endif