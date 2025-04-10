
global idt_load
extern timer_handler

idt_load:
    lidt [idtp]
    ret

global isr32
isr32:
    call timer_handler
    mov al, 0x20
    out 0x20, al 
    iret

global isr33
extern keyboard_handler

isr33:
    call keyboard_handler
    mov al, 0x20
    out 0x20, al
    iret
