#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include "../exceptions/interrupts.h"

typedef void (*isr_t)(registers_t*);

extern isr_t interrupt_handlers[256];

void register_interrupt_handler(
    unsigned char n,
    isr_t handler
);

#endif