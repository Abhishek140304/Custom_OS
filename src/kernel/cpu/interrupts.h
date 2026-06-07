/*
Description: Defines the data structure that perfectly mirrors how our Assembly interrupt wrappers push the CPU state onto the stack before calling our C handler.
*/


#ifndef INTERRUPTS_H
#define INTERRUPTS_H


// A 1:1 mapping of the CPU stack when an interrupt occurs. The order of these variables is extremely strict because they represent the exact reverse order that data was pushed onto the stack.
typedef struct{
    unsigned int ds;

    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;

    unsigned int int_no;
    unsigned int err_code;

    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;

} registers_t;

void isr_handler(registers_t* regs);

#endif