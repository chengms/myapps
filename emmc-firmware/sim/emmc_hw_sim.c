#include "emmc_hw_if.h"
#include "emmc_regs.h"

#include <stdio.h>
#include <string.h>

#define EMMC_SIM_BLOCKS (4U * 1024U) /* 4K blocks -> 2 MiB */

typedef struct {
    uint32_t regs[32];
    unsigned char storage[EMMC_SIM_BLOCKS * EMMC_BLOCK_SIZE_BYTES];
} emmc_hw_state_t;

static emmc_hw_state_t g_hw_state;
static size_t g_capacity_blocks = EMMC_SIM_BLOCKS;

static inline uint32_t *reg_ptr(uint32_t offset)
{
    return &g_hw_state.regs[emmc_reg_index(offset)];
}

static void set_status(uint32_t mask)
{
    uint32_t *status = reg_ptr(EMMC_REG_STATUS);
    *status |= mask;
}

static void clear_status(uint32_t mask)
{
    uint32_t *status = reg_ptr(EMMC_REG_STATUS);
    *status &= ~mask;
}

static void set_interrupt(uint32_t mask)
{
    uint32_t *int_status = reg_ptr(EMMC_REG_INT_STATUS);
    uint32_t enable = *reg_ptr(EMMC_REG_INT_ENABLE);
    *int_status |= (mask & enable);
}

static void clear_interrupt(uint32_t mask)
{
    uint32_t *int_status = reg_ptr(EMMC_REG_INT_STATUS);
    *int_status &= ~mask;
}

int emmc_hw_init(void)
{
    memset(&g_hw_state, 0, sizeof(g_hw_state));
    *reg_ptr(EMMC_REG_BLOCK_SIZE) = EMMC_BLOCK_SIZE_BYTES;
    *reg_ptr(EMMC_REG_BLOCK_COUNT) = 0U;
    *reg_ptr(EMMC_REG_STATUS) = EMMC_STATUS_READY;
    *reg_ptr(EMMC_REG_INT_ENABLE) = EMMC_INT_CMD_COMPLETE | EMMC_INT_DATA_COMPLETE | EMMC_INT_ERROR;
    return 0;
}

void emmc_hw_reset(void)
{
    emmc_hw_init();
}

uint32_t emmc_hw_read_reg(uint32_t offset)
{
    if (offset % sizeof(uint32_t) != 0U) {
        fprintf(stderr, "[emmc_hw_sim] Attempted unaligned read at 0x%08x\n", offset);
        return 0U;
    }

    return *reg_ptr(offset);
}

void emmc_hw_write_reg(uint32_t offset, uint32_t value)
{
    if (offset % sizeof(uint32_t) != 0U) {
        fprintf(stderr, "[emmc_hw_sim] Attempted unaligned write at 0x%08x\n", offset);
        return;
    }

    switch (offset) {
    case EMMC_REG_RESET:
        if ((value & EMMC_RESET_ALL) != 0U) {
            emmc_hw_reset();
        }
        break;
    case EMMC_REG_CMD:
        clear_status(EMMC_STATUS_READY);
        set_status(EMMC_STATUS_CMD_INHIBIT);
        *reg_ptr(offset) = value;
        set_interrupt(EMMC_INT_CMD_COMPLETE);
        clear_status(EMMC_STATUS_CMD_INHIBIT);
        set_status(EMMC_STATUS_READY);
        break;
    case EMMC_REG_INT_STATUS:
        clear_interrupt(value);
        break;
    default:
        *reg_ptr(offset) = value;
        break;
    }
}

static inline unsigned char *block_address(uint32_t lba)
{
    return &g_hw_state.storage[(size_t)lba * EMMC_BLOCK_SIZE_BYTES];
}

int emmc_hw_read_blocks(uint32_t lba, void *buffer, size_t block_count)
{
    if (buffer == NULL || block_count == 0U) {
        return -1;
    }
    if (((size_t)lba + block_count) > g_capacity_blocks) {
        set_status(EMMC_STATUS_ERROR);
        set_interrupt(EMMC_INT_ERROR);
        return -2;
    }

    clear_status(EMMC_STATUS_READY);
    set_status(EMMC_STATUS_DATA_INHIBIT);

    memcpy(buffer, block_address(lba), block_count * EMMC_BLOCK_SIZE_BYTES);

    clear_status(EMMC_STATUS_DATA_INHIBIT);
    set_status(EMMC_STATUS_READY);
    set_interrupt(EMMC_INT_DATA_COMPLETE);
    return 0;
}

int emmc_hw_write_blocks(uint32_t lba, const void *buffer, size_t block_count)
{
    if (buffer == NULL || block_count == 0U) {
        return -1;
    }
    if (((size_t)lba + block_count) > g_capacity_blocks) {
        set_status(EMMC_STATUS_ERROR);
        set_interrupt(EMMC_INT_ERROR);
        return -2;
    }

    clear_status(EMMC_STATUS_READY);
    set_status(EMMC_STATUS_DATA_INHIBIT);

    memcpy(block_address(lba), buffer, block_count * EMMC_BLOCK_SIZE_BYTES);

    clear_status(EMMC_STATUS_DATA_INHIBIT);
    set_status(EMMC_STATUS_READY);
    set_interrupt(EMMC_INT_DATA_COMPLETE);
    return 0;
}

size_t emmc_hw_capacity_blocks(void)
{
    return g_capacity_blocks;
}
