/*
Description: Defines the high-level console interface for the kernel. This layer abstracts away the low-level VGA hardware details, providing simple functions for standard text output.
*/

#ifndef CONSOLE_H
#define CONSOLE_H

void printk(const char* str);

#endif