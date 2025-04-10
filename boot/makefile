
AS = nasm
CC = gcc
LD = ld

ASFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -c
LDFLAGS = -m elf_i386 -T linker.ld

BOOTLOADER_SRC = bootloader.asm
ASM_SRC = kernel.asm
IDT_ASM_SRC = idt.asm

C_SRCS = kernel.c shell.c fs.c ata.c pci.c utils.c idt.c timer.c keyboard.c memory.c
OBJS = kernel_entry.o idt_asm.o $(C_SRCS:.c=.o)

all: os.img iso

bootloader.bin: $(BOOTLOADER_SRC)
	$(AS) $< -f bin -o $@

kernel_entry.o: $(ASM_SRC)
	$(AS) $(ASFLAGS) $< -o $@

idt_asm.o: $(IDT_ASM_SRC)
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

gos.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

os.img: bootloader.bin gos.bin
	dd if=/dev/zero of=os.img bs=512 count=10000
	dd if=bootloader.bin of=os.img conv=notrunc
	dd if=gos.bin of=os.img bs=512 seek=1 conv=notrunc

iso: gos.bin
	mkdir -p iso_root
	cp gos.bin iso_root/boot.img
	xorriso -as mkisofs -b boot.img -no-emul-boot -boot-load-size 4 -boot-info-table -o gos.iso iso_root

clean:
	rm -f *.o *.bin gos.bin os.img gos.iso
	rm -rf iso_root

run: os.img
	qemu-system-i386 -drive format=raw,file=os.img

runiso: gos.iso
	qemu-system-i386 -cdrom gos.iso
