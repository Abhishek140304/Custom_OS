#include "timer.h"
#include "../console/console.h"

unsigned int tick = 0;

void timer_callback(){
    tick++;

    printk_hex(tick);
    printk("Tick\n");
}

void timer_init(){

}