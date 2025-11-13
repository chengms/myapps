#include <stdio.h>
#include <string.h>

#include "emmc_controller.h"

#define BLOCK_SIZE 512U

static void dump_block_preview(const uint8_t *buffer, size_t length) {
  printf("Preview: ");
  for (size_t i = 0; i < length && i < 16U; ++i) {
    printf("%02X ", buffer[i]);
  }
  printf("...\n");
}

int main(void) {
  emmc_config_t config = {
      .clock_khz = 26000U,
      .bus_width = 4U,
      .enable_high_speed = true,
      .boot_timeout_ms = 500U,
  };

  if (emmc_init(&config) != EMMC_STATUS_OK) {
    printf("[TEST] 初始化失败\n");
    return 1;
  }

  printf("[TEST] 初始化成功，RCA=%u\n", emmc_controller_get()->rca);

  uint8_t read_buffer[BLOCK_SIZE];
  memset(read_buffer, 0, sizeof(read_buffer));

  if (emmc_read_block(0, read_buffer, BLOCK_SIZE) != EMMC_STATUS_OK) {
    printf("[TEST] 读取块失败\n");
    return 1;
  }
  printf("[TEST] 读取块0成功\n");
  dump_block_preview(read_buffer, sizeof(read_buffer));

  uint8_t write_buffer[BLOCK_SIZE];
  for (size_t i = 0; i < BLOCK_SIZE; ++i) {
    write_buffer[i] = (uint8_t)(i & 0xFFU);
  }

  if (emmc_write_block(1, write_buffer, BLOCK_SIZE) != EMMC_STATUS_OK) {
    printf("[TEST] 写入块失败\n");
    return 1;
  }
  printf("[TEST] 写入块1成功\n");

  uint8_t verify_buffer[BLOCK_SIZE];
  memset(verify_buffer, 0, sizeof(verify_buffer));
  if (emmc_read_block(1, verify_buffer, BLOCK_SIZE) != EMMC_STATUS_OK) {
    printf("[TEST] 验证读取失败\n");
    return 1;
  }

  if (memcmp(write_buffer, verify_buffer, BLOCK_SIZE) != 0) {
    printf("[TEST] 数据校验失败\n");
    return 1;
  }

  printf("[TEST] 数据校验通过\n");
  dump_block_preview(verify_buffer, sizeof(verify_buffer));
  return 0;
}
