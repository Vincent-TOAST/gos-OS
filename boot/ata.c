
#include <stdint.h>


static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline void io_wait(void) {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}


#define ATA_DATA       0x1F0
#define ATA_SECCOUNT0  0x1F2
#define ATA_LBA0       0x1F3
#define ATA_LBA1       0x1F4
#define ATA_LBA2       0x1F5
#define ATA_COMMAND    0x1F7
#define ATA_STATUS     0x1F7

#define ATA_CMD_WRITE_SECTORS 0x30


void ata_write_sector(uint32_t lba, const void* buffer) {
    const uint16_t* buf = (const uint16_t*)buffer;

    outb(ATA_SECCOUNT0, 1);
    outb(ATA_LBA0, (uint8_t)(lba));
    outb(ATA_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_LBA2, (uint8_t)(lba >> 16));
    outb(ATA_COMMAND, ATA_CMD_WRITE_SECTORS);

    while (!(inb(ATA_STATUS) & 0x08));  

    for (int i = 0; i < 256; i++) {
        outw(ATA_DATA, buf[i]);
    }

    io_wait();  
}
