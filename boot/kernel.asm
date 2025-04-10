[bits 32]

section .multiboot
align 4
    dd 0x1BADB002        
    dd 0x0              
    dd -(0x1BADB002 + 0x0) 

section .text
global _start
extern kernel_main

_start:
    
    mov edi, 0xb8000       
    mov eax, 0x07204F42    ; "BO"
    stosd
    mov eax, 0x07204E49    ; "IN"
    stosd
    mov eax, 0x07204720    ; "G "
    stosd
    mov eax, 0x07204D59    ; "gos"
    stosd
    mov eax, 0x0720534F    ; "OS"
    stosd

    mov eax, ebx
    push eax
    
    call kernel_main

.halt:
    hlt
    jmp .halt

