
#pragma once
#include <stdint.h>

void idt_init();
void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void isr32();
extern void isr33();
extern void idt_load();
