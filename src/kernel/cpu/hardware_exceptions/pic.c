/*
Description: Remaps the Intel 8259 Programmable Interrupt Controllers (PICs) so that hardware interrupts (IRQs) do not collide with CPU exceptions (ISRs) in the IDT.
*/


#include "pic.h"
#include "ports.h"

void pic_remap(){
    // 1. Save Existing Masks
    // The PICs might already have certain interrupts disabled (masked). We read their current states from the data ports so we can restore them later.
    unsigned char master_mask = inb(0x21);
    unsigned char slave_mask = inb(0xA1);


    // 2. ICW1: Initialization Command
    // Send 0x11 to the command ports (0x20 and 0xA0). This tells the PICs to start the initialization sequence and wait for 3 more configuration bytes.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);


    // 3. ICW2: Vector Offsets (The Actual Remapping)
    // Tell the Master PIC to start its interrupts at IDT entry 32 (0x20). Tell the Slave PIC to start its interrupts at IDT entry 40 (0x28).
    outb(0x21, 0x20);
    outb(0xA1, 0x28);


    // 4. ICW3: Cascading Setup
    // Tell the Master PIC that it has a Slave PIC wired to its IRQ2 pin (0x04 = 00000100 in binary).
    outb(0x21, 0x04);
    outb(0xA1, 0x02);


    // 5. ICW4: Environment Mode
    // Send 0x01 to tell both PICs we are operating in 8086/88 (standard x86) mode.
    outb(0x21, 0x01);
    outb(0xA1, 0x01);


    // 6. Restore Masks
    // Push the saved masks back to the data ports to finish the setup.
    outb(0x21, master_mask);
    outb(0xA1, slave_mask);
}