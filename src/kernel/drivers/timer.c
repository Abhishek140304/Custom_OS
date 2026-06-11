#include "timer.h"
#include "../cpu/interrupt_manager/interrupt_manager.h"

unsigned int tick = 0;

static void timer_callback(registers_t* regs){
    tick++;
}

void timer_init(){
    register_interrupt_handler(32, timer_callback);
}