/*
Description: Provides low-level hardware communication functions. These wrappers allow our C code to read and write bytes directly to the CPU's I/O ports using inline assembly.
*/


#include "ports.h"

// Reads a single byte of data from the specified I/O port.
unsigned char inb(unsigned short port){
    unsigned char result;

    __asm__ volatile(
        "inb %1, %0"        // Read from port (%1) into register (%0)
        : "=a"(result)      // OUTPUT: Tell GCC to put the 'eax/ax/al' register into our 'result' variable.
        : "Nd"(port)        // // INPUT: Tell GCC to put our 'port' variable into the 'edx/dx/dl' register.
    );

    return result;
}

// Writes a single byte of data to the specified I/O port.
void outb(unsigned short port, unsigned char data){
    __asm__ volatile(
        "outb %0, %1"           // Write data (%0) to port (%1)
        :                       // OUTPUT: None. We are sending data, not receiving it.
        : "a"(data), "Nd"(port) // INPUT: Put 'data' into 'eax/al', and 'port' into 'edx/dx'.
    );
}