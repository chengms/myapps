#ifndef EMMC_FIRMWARE_HAL_INCLUDE_HAL_EMMC_H_
#define EMMC_FIRMWARE_HAL_INCLUDE_HAL_EMMC_H_

#include <stdint.h>
#include <stdbool.h>
#include "emmc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

// 针对不同平台实现的硬件抽象接口，在仿真模式下由 sim 层提供支持。

bool hal_emmc_init(void);
void hal_emmc_reset(void);
uint32_t hal_emmc_reg_read(emmc_register_t reg);
void hal_emmc_reg_write(emmc_register_t reg, uint32_t value);
void hal_emmc_delay_us(uint32_t microseconds);
void hal_emmc_log(const char *fmt, ...);

// 数据 FIFO 操作接口（仿真环境使用内存 buffer 实现）
uint32_t hal_emmc_fifo_read(void);
void hal_emmc_fifo_write(uint32_t value);
void hal_emmc_fifo_flush(void);

#ifdef __cplusplus
}
#endif

#endif  // EMMC_FIRMWARE_HAL_INCLUDE_HAL_EMMC_H_
