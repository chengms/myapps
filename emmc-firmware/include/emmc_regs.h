#ifndef EMMC_REGS_H
#define EMMC_REGS_H

#include <stdint.h>

#define EMMC_REG_SYSTEM_CTRL      0x00U
#define EMMC_REG_CMD              0x04U
#define EMMC_REG_ARG              0x08U
#define EMMC_REG_RESP0            0x0CU
#define EMMC_REG_RESP1            0x10U
#define EMMC_REG_RESP2            0x14U
#define EMMC_REG_RESP3            0x18U
#define EMMC_REG_DATA             0x1CU
#define EMMC_REG_STATUS           0x20U
#define EMMC_REG_INT_STATUS       0x24U
#define EMMC_REG_INT_ENABLE       0x28U
#define EMMC_REG_BLOCK_SIZE       0x2CU
#define EMMC_REG_BLOCK_COUNT      0x30U
#define EMMC_REG_CLOCK_CTRL       0x34U
#define EMMC_REG_RESET            0x38U

#define EMMC_STATUS_READY         (1U << 0)
#define EMMC_STATUS_CMD_INHIBIT   (1U << 1)
#define EMMC_STATUS_DATA_INHIBIT  (1U << 2)
#define EMMC_STATUS_ERROR         (1U << 15)

#define EMMC_INT_CMD_COMPLETE     (1U << 0)
#define EMMC_INT_DATA_COMPLETE    (1U << 1)
#define EMMC_INT_ERROR            (1U << 15)

#define EMMC_RESET_ALL            (1U << 0)
#define EMMC_RESET_CMD            (1U << 1)
#define EMMC_RESET_DATA           (1U << 2)

#define EMMC_BLOCK_SIZE_BYTES     512U

static inline uint32_t emmc_reg_index(uint32_t offset)
{
    return offset / sizeof(uint32_t);
}

#endif  /* EMMC_REGS_H */
