
[org 0x7C00]
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

   
    mov si, 0x1000      
    mov bx, 0            
    call load_kernel

   
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:pm_start    

; ------------------------------
; Protected Mode Begins Here
; ------------------------------
[bits 32]
pm_start:
    mov ax, 0x10       
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x100000         

; ------------------------------
; Load Sectors from Disk
; ------------------------------
[bits 16]
load_kernel:
    pusha
    mov ah, 0x02        
    mov al, 19           
    mov ch, 0            
    mov cl, 2            
    mov dh, 0            
    mov dl, 0x80         
    mov bx, 0
    mov es, si
    int 0x13
    jc disk_error
    popa
    ret

disk_error:
    mov si, error_msg
.print:
    lodsb
    or al, al
    jz .halt
    mov ah, 0x0E
    int 0x10
    jmp .print
.halt:
    cli
    hlt
    jmp .halt

error_msg: db "Disk read failed", 0

; ------------------------------
; GDT (Global Descriptor Table)
; ------------------------------
gdt_start:
 
    dd 0x0
    dd 0x0
 
    dw 0xFFFF, 0x0000, 0x9A00, 0xCF00

    dw 0xFFFF, 0x0000, 0x9200, 0xCF00

gdt_descriptor:
    dw gdt_descriptor - gdt_start - 1
    dd gdt_start

times 510 - ($ - $$) db 0
dw 0xAA55
