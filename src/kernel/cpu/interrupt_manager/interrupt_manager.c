/*
// Description: Implements a dynamic registry for interrupt handlers. This allows individual hardware drivers (like the keyboard) to "subscribe" to specific interrupts without having to modify the core kernel interrupt dispatcher.
*/

#include "interrupt_manager.h"

// 'isr_t' is a custom type definition for a function pointer that takes 'registers_t*' as an argument. We create an array of 256 of these pointers, mapping perfectly to the 256 possible IDT entries. If a slot is 0 (NULL), it means no driver has registered to listen to that interrupt.
isr_t interrupt_handlers[256];


// Called by a device driver during its initialization phase to hook its callback function into the system.
void register_interrupt_handler(unsigned char n, isr_t handler){
    interrupt_handlers[n] = handler;
}