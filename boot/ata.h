
#pragma once
#include <stdint.h>

void ata_read_sector(uint32_t lba, void* buffer);
void ata_write_sector(uint32_t lba, const void* buffer);  // NEW
