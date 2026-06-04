#ifndef INTERRUPTS_H
#define INTERRUPTS_H

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