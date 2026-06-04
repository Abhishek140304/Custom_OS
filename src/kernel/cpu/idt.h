#ifndef IDT_H
#define IDT_H

typedef struct{
    unsigned short base_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_ptr_t;

void idt_init();

#endif