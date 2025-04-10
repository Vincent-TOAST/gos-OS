
#include "keyboard.h"
#include "utils.h"

#define KEYBOARD_PORT 0x60
static char buffer = 0;

static char keymap[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' '
};

void keyboard_handler() {
    uint8_t scancode;
    __asm__ volatile("inb %1, %0" : "=a"(scancode) : "Nd"(KEYBOARD_PORT));

    if (scancode < 0x80) {
        buffer = keymap[scancode];
    }

  
    __asm__ volatile("movb $0x20, %al; outb %al, $0x20");
}

char get_char_from_buffer() {
    char ch = buffer;
    buffer = 0;
    return ch;
}

void keyboard_init() {}

extern char get_char_from_buffer();

   
