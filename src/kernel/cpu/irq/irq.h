#ifndef IRQ_H
#define IRQ_H

#include "../exceptions/interrupts.h"

void irq_init();

void irq_handler(registers_t* reg);

#endif