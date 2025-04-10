
#include "timer.h"
#include "utils.h"



volatile uint32_t tick = 0;

static void timer_callback() {
    tick++;
}

uint32_t get_ticks() {
    return tick;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void timer_init() {
    
    uint32_t divisor = 1193180 / 100; 

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void timer_handler() {
}
