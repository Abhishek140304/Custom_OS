/*
Description: Controls the Intel Programmable Interval Timer (PIT). It sets up a predictable system heartbeat and tracks the total uptime of the operating system.
*/

#include "timer.h"
#include "../cpu/io/ports.h"
#include "../cpu/interrupt_manager/interrupt_manager.h"

volatile unsigned int ticks = 0;

static void timer_callback(registers_t* regs){
    ticks++;
}

unsigned int timer_get_ticks(){
    return ticks;
}

void timer_init(){
    register_interrupt_handler(32, timer_callback);

    unsigned int frequency = 100;

    // The internal hardware clock oscillates at exactly 1,193,180 Hz. To get our desired frequency (e.g., 100 Hz), we must divide the hardware frequency by our target frequency.
    unsigned int divisor = 1193180 / frequency;

    /*
     * Send the command byte 0x36 to the PIT command port (0x43).
     * 0x36 in binary is 0011 0110, which translates to:
     * Bits 6-7 (00): Use Channel 0
     * Bits 4-5 (11): Access mode (Expect both Low byte and High byte)
     * Bits 1-3 (011): Operating Mode 3 (Square Wave Generator)
     * Bit 0 (0): 16-bit binary mode
     */
    outb(0x43, 0x36);

    // The PIT data port (0x40) only accepts 8 bits at a time, but our divisor is 16 bits. We have to send it in two pieces.
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void sleep(unsigned int ms){
    unsigned int start = ticks;

    unsigned int wait_ticks = ms / 10;

    if(wait_ticks == 0) wait_ticks = 1;

    while((ticks - start) < wait_ticks);
}