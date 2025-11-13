#ifndef EMMC_HW_IF_H
#define EMMC_HW_IF_H

#include <stddef.h>
#include <stdint.h>

int emmc_hw_init(void);
void emmc_hw_reset(void);
uint32_t emmc_hw_read_reg(uint32_t offset);
void emmc_hw_write_reg(uint32_t offset, uint32_t value);
int emmc_hw_read_blocks(uint32_t lba, void *buffer, size_t block_count);
int emmc_hw_write_blocks(uint32_t lba, const void *buffer, size_t block_count);
size_t emmc_hw_capacity_blocks(void);

#endif /* EMMC_HW_IF_H */
