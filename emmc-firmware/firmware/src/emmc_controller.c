#include "emmc_controller.h"

#include <string.h>

static emmc_controller_t g_controller;

static emmc_status_t emmc_wait_for_interrupt(uint32_t mask, uint32_t timeout_us) {
  const uint32_t poll_interval_us = 10U;
  uint32_t waited = 0U;
  while (waited <= timeout_us) {
    uint32_t status = hal_emmc_reg_read(EMMC_REG_INT_STATUS);
    if ((status & mask) != 0U) {
      return EMMC_STATUS_OK;
    }
    if ((status & EMMC_INT_ERROR) != 0U) {
      return EMMC_STATUS_HW_FAILURE;
    }
    hal_emmc_delay_us(poll_interval_us);
    waited += poll_interval_us;
  }
  return EMMC_STATUS_TIMEOUT;
}

static void emmc_clear_interrupt(uint32_t mask) {
  hal_emmc_reg_write(EMMC_REG_INT_STATUS, mask);
}

static emmc_status_t emmc_issue_command(uint8_t cmd_idx, uint32_t argument,
                                        uint32_t cmd_flags, bool long_response,
                                        uint32_t *response) {
  hal_emmc_reg_write(EMMC_REG_CMD_ARG, argument);
  uint32_t command = (uint32_t)(cmd_idx & EMMC_CMD_INDEX_MASK) | EMMC_CMD_START |
                     cmd_flags;
  if (response != NULL) {
    command |= EMMC_CMD_RESP_EXPECT;
    if (long_response) {
      command |= EMMC_CMD_RESP_LONG;
    }
  }
  emmc_clear_interrupt(EMMC_INT_CMD_DONE | EMMC_INT_ERROR);
  hal_emmc_reg_write(EMMC_REG_CMD_TRANSFER, command);
  emmc_status_t status = emmc_wait_for_interrupt(EMMC_INT_CMD_DONE,
                                                 1000U);
  if (status != EMMC_STATUS_OK) {
    return status;
  }
  if (response != NULL) {
    response[0] = hal_emmc_reg_read(EMMC_REG_RESP0);
    if (long_response) {
      response[1] = hal_emmc_reg_read(EMMC_REG_RESP1);
      response[2] = hal_emmc_reg_read(EMMC_REG_RESP2);
      response[3] = hal_emmc_reg_read(EMMC_REG_RESP3);
    }
  }
  emmc_clear_interrupt(EMMC_INT_CMD_DONE);
  return EMMC_STATUS_OK;
}

emmc_controller_t *emmc_controller_get(void) {
  return &g_controller;
}

emmc_status_t emmc_init(const emmc_config_t *config) {
  if (config == NULL) {
    return EMMC_STATUS_INVALID_ARG;
  }
  memset(&g_controller, 0, sizeof(g_controller));
  if (!hal_emmc_init()) {
    return EMMC_STATUS_HW_FAILURE;
  }

  hal_emmc_reg_write(EMMC_REG_PWR_CTRL, EMMC_PWR_CTRL_ON);
  hal_emmc_reg_write(EMMC_REG_SYS_CTRL, EMMC_SYS_CTRL_RST);
  hal_emmc_reg_write(EMMC_REG_CLK_CTRL, EMMC_CLK_CTRL_ENABLE);
  hal_emmc_reg_write(EMMC_REG_HOST_CTRL,
                     (config->bus_width == 8
                          ? EMMC_HOST_CTRL_BUS_WIDTH_8BIT
                          : (config->bus_width == 4
                                 ? EMMC_HOST_CTRL_BUS_WIDTH_4BIT
                                 : EMMC_HOST_CTRL_BUS_WIDTH_1BIT)) |
                         (config->enable_high_speed ? EMMC_HOST_CTRL_HIGH_SPEED
                                                    : 0));

  uint32_t response[4] = {0};
  emmc_status_t status = emmc_issue_command(0, 0, 0, false, NULL);
  if (status != EMMC_STATUS_OK) {
    return status;
  }

  status = emmc_issue_command(1, 0x40FF8000U, 0, false, response);
  if (status != EMMC_STATUS_OK) {
    return status;
  }

  status = emmc_issue_command(2, 0, 0, true, response);
  if (status != EMMC_STATUS_OK) {
    return status;
  }

  status = emmc_issue_command(3, 0, 0, false, response);
  if (status != EMMC_STATUS_OK) {
    return status;
  }
  g_controller.rca = response[0] >> 16;

  status = emmc_issue_command(7, g_controller.rca << 16, 0, false, NULL);
  if (status != EMMC_STATUS_OK) {
    return status;
  }

  g_controller.bus_width = config->bus_width;
  g_controller.clock_khz = config->clock_khz;
  g_controller.high_speed = config->enable_high_speed;
  g_controller.initialized = true;
  return EMMC_STATUS_OK;
}

emmc_status_t emmc_send_command(uint8_t cmd_idx, uint32_t argument,
                                bool long_response, uint32_t *response) {
  if (!g_controller.initialized) {
    return EMMC_STATUS_INVALID_ARG;
  }
  return emmc_issue_command(cmd_idx, argument, 0, long_response, response);
}

static emmc_status_t emmc_wait_data_done(void) {
  emmc_status_t status = emmc_wait_for_interrupt(EMMC_INT_DATA_DONE,
                                                 5000U);
  if (status == EMMC_STATUS_OK) {
    emmc_clear_interrupt(EMMC_INT_DATA_DONE);
  }
  return status;
}

emmc_status_t emmc_read_block(uint32_t lba, uint8_t *buffer, size_t length) {
  if (!g_controller.initialized || buffer == NULL || length == 0U ||
      (length % sizeof(uint32_t)) != 0U) {
    return EMMC_STATUS_INVALID_ARG;
  }
  hal_emmc_reg_write(EMMC_REG_DATA_LENGTH, (uint32_t)length);
  hal_emmc_reg_write(EMMC_REG_DATA_CTRL,
                     EMMC_DATA_CTRL_ENABLE | EMMC_DATA_CTRL_DIR_READ);
  emmc_status_t status = emmc_issue_command(17, lba,
                                           EMMC_CMD_DATA_PRESENT |
                                               EMMC_CMD_READ,
                                           false, NULL);
  if (status != EMMC_STATUS_OK) {
    return status;
  }

  for (size_t offset = 0; offset < length; offset += sizeof(uint32_t)) {
    uint32_t word = hal_emmc_fifo_read();
    buffer[offset + 0] = (uint8_t)(word & 0xFFU);
    buffer[offset + 1] = (uint8_t)((word >> 8) & 0xFFU);
    buffer[offset + 2] = (uint8_t)((word >> 16) & 0xFFU);
    buffer[offset + 3] = (uint8_t)((word >> 24) & 0xFFU);
  }
  return emmc_wait_data_done();
}

emmc_status_t emmc_write_block(uint32_t lba, const uint8_t *buffer,
                               size_t length) {
  if (!g_controller.initialized || buffer == NULL || length == 0U ||
      (length % sizeof(uint32_t)) != 0U) {
    return EMMC_STATUS_INVALID_ARG;
  }
  hal_emmc_reg_write(EMMC_REG_DATA_LENGTH, (uint32_t)length);
  hal_emmc_reg_write(EMMC_REG_DATA_CTRL, EMMC_DATA_CTRL_ENABLE);
  emmc_status_t status = emmc_issue_command(24, lba,
                                           EMMC_CMD_DATA_PRESENT,
                                           false, NULL);
  if (status != EMMC_STATUS_OK) {
    return status;
  }

  for (size_t offset = 0; offset < length; offset += sizeof(uint32_t)) {
    uint32_t word = (uint32_t)buffer[offset] |
                    ((uint32_t)buffer[offset + 1] << 8) |
                    ((uint32_t)buffer[offset + 2] << 16) |
                    ((uint32_t)buffer[offset + 3] << 24);
    hal_emmc_fifo_write(word);
  }
  return emmc_wait_data_done();
}
