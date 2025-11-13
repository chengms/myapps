#ifndef EMMC_FIRMWARE_INCLUDE_EMMC_CONTROLLER_H_
#define EMMC_FIRMWARE_INCLUDE_EMMC_CONTROLLER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "hal_emmc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  EMMC_STATUS_OK = 0,
  EMMC_STATUS_TIMEOUT,
  EMMC_STATUS_INVALID_ARG,
  EMMC_STATUS_HW_FAILURE,
  EMMC_STATUS_BUSY,
  EMMC_STATUS_UNSUPPORTED
} emmc_status_t;

// eMMC 初始化配置
typedef struct {
  uint32_t clock_khz;    // 目标时钟（kHz）
  uint8_t bus_width;     // 1/4/8-bit
  bool enable_high_speed;
  uint32_t boot_timeout_ms;
} emmc_config_t;

// 控制器主状态
typedef struct {
  bool initialized;
  bool high_speed;
  uint8_t bus_width;
  uint32_t clock_khz;
  uint32_t rca;  // Relative card address (仿真默认固定)
} emmc_controller_t;

emmc_controller_t *emmc_controller_get(void);
emmc_status_t emmc_init(const emmc_config_t *config);
emmc_status_t emmc_send_command(uint8_t cmd_idx, uint32_t argument,
                                bool long_response, uint32_t *response);
emmc_status_t emmc_read_block(uint32_t lba, uint8_t *buffer, size_t length);
emmc_status_t emmc_write_block(uint32_t lba, const uint8_t *buffer,
                               size_t length);

#ifdef __cplusplus
}
#endif

#endif  // EMMC_FIRMWARE_INCLUDE_EMMC_CONTROLLER_H_
