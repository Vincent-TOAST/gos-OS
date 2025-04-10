[bits 32]

global idt_load
global isr32
global isr33

extern idtp 
extern timer_handler
extern keyboard_handler

idt_load:
    lidt [idtp]
    ret

isr32:
    call timer_handler
    mov al, 0x20
    out 0x20, al
    iret

isr33:
    call keyboard_handler
    mov al, 0x20
    out 0x20, al
    iret
