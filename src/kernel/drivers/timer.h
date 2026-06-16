#ifndef TIMER_H
#define TIMER_H

#include "../cpu/exceptions/interrupts.h"

void timer_init();

static void timer_callback(registers_t* regs);

unsigned int timer_get_ticks();

void sleep(unsigned int ms);

#endif