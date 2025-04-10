
#include "keyboard.h"
#include "utils.h"
#include "idt.h"
#include <stdint.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 128

static char keymap[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0,'*', 0,' '
};


static char kb_buffer[KEYBOARD_BUFFER_SIZE];
static int head = 0;
static int tail = 0;

void keyboard_handler() {
    uint8_t scancode;
    __asm__ volatile("inb %1, %0" : "=a"(scancode) : "Nd"(KEYBOARD_PORT));

    if (scancode < 0x80) {
        char key = keymap[scancode];
        if (key) {
            int next = (head + 1) % KEYBOARD_BUFFER_SIZE;
            if (next != tail) {  
                kb_buffer[head] = key;
                head = next;
            }
        }
    }
    __asm__ volatile("movb $0x20, %al; outb %al, $0x20");
}

char get_char_from_buffer() {
    if (head == tail) return 0;
    char ch = kb_buffer[tail];
    tail = (tail + 1) % KEYBOARD_BUFFER_SIZE;
    return ch;
}

void keyboard_init() {
    idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E); 
}
