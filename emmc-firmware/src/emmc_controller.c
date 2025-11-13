#include "emmc_controller.h"
#include "emmc_hw_if.h"
#include "emmc_regs.h"

#include <stddef.h>
#include <string.h>

#define EMMC_CMD_GO_IDLE_STATE            0U
#define EMMC_CMD_SEND_OP_COND             1U
#define EMMC_CMD_ALL_SEND_CID             2U
#define EMMC_CMD_SET_RELATIVE_ADDR        3U
#define EMMC_CMD_SELECT_CARD              7U
#define EMMC_CMD_SEND_IF_COND             8U
#define EMMC_CMD_READ_SINGLE_BLOCK        17U
#define EMMC_CMD_WRITE_SINGLE_BLOCK       24U

static emmc_result_t validate_controller_inputs(emmc_controller_t *controller,
                                                uint32_t lba,
                                                size_t block_count)
{
    if (controller == NULL) {
        return EMMC_RESULT_INVALID_ARG;
    }
    if (controller->initialised == 0) {
        return EMMC_RESULT_NOT_INITIALISED;
    }
    if (block_count == 0U) {
        return EMMC_RESULT_INVALID_ARG;
    }
    if (((size_t)lba + block_count) > controller->config.capacity_blocks) {
        return EMMC_RESULT_OUT_OF_RANGE;
    }

    return EMMC_RESULT_OK;
}

static void issue_command(uint32_t cmd_index, uint32_t argument)
{
    emmc_hw_write_reg(EMMC_REG_ARG, argument);
    emmc_hw_write_reg(EMMC_REG_CMD, cmd_index);
}

emmc_result_t emmc_controller_init(emmc_controller_t *controller,
                                   const emmc_controller_config_t *config)
{
    if (controller == NULL) {
        return EMMC_RESULT_INVALID_ARG;
    }

    emmc_controller_config_t local_cfg = {
        .clock_hz = 25U * 1000U * 1000U, /* 25 MHz 默认 */
        .bus_width_bits = 4U,
        .capacity_blocks = 0U,
    };

    if (config != NULL) {
        local_cfg = *config;
    }

    if (local_cfg.bus_width_bits != 1U &&
        local_cfg.bus_width_bits != 4U &&
        local_cfg.bus_width_bits != 8U) {
        return EMMC_RESULT_INVALID_ARG;
    }

    if (emmc_hw_init() != 0) {
        return EMMC_RESULT_HW_ERROR;
    }

    size_t hw_capacity = emmc_hw_capacity_blocks();
    if (local_cfg.capacity_blocks == 0U || local_cfg.capacity_blocks > hw_capacity) {
        local_cfg.capacity_blocks = hw_capacity;
    }

    emmc_hw_write_reg(EMMC_REG_CLOCK_CTRL, local_cfg.clock_hz);
    emmc_hw_write_reg(EMMC_REG_BLOCK_SIZE, EMMC_BLOCK_SIZE_BYTES);

    controller->config = local_cfg;
    controller->initialised = 1;

    issue_command(EMMC_CMD_GO_IDLE_STATE, 0U);
    issue_command(EMMC_CMD_SEND_IF_COND, 0x1AAU);
    issue_command(EMMC_CMD_SEND_OP_COND, 0x40FF8000U);

    return EMMC_RESULT_OK;
}

emmc_result_t emmc_controller_read_blocks(emmc_controller_t *controller,
                                          uint32_t lba,
                                          void *buffer,
                                          size_t block_count)
{
    emmc_result_t status = validate_controller_inputs(controller, lba, block_count);
    if (status != EMMC_RESULT_OK) {
        return status;
    }
    if (buffer == NULL) {
        return EMMC_RESULT_INVALID_ARG;
    }

    issue_command(EMMC_CMD_READ_SINGLE_BLOCK, lba);

    if (emmc_hw_read_blocks(lba, buffer, block_count) != 0) {
        return EMMC_RESULT_HW_ERROR;
    }

    emmc_hw_write_reg(EMMC_REG_BLOCK_COUNT, (uint32_t)block_count);
    return EMMC_RESULT_OK;
}

emmc_result_t emmc_controller_write_blocks(emmc_controller_t *controller,
                                           uint32_t lba,
                                           const void *buffer,
                                           size_t block_count)
{
    emmc_result_t status = validate_controller_inputs(controller, lba, block_count);
    if (status != EMMC_RESULT_OK) {
        return status;
    }
    if (buffer == NULL) {
        return EMMC_RESULT_INVALID_ARG;
    }

    issue_command(EMMC_CMD_WRITE_SINGLE_BLOCK, lba);

    if (emmc_hw_write_blocks(lba, buffer, block_count) != 0) {
        return EMMC_RESULT_HW_ERROR;
    }

    emmc_hw_write_reg(EMMC_REG_BLOCK_COUNT, (uint32_t)block_count);
    return EMMC_RESULT_OK;
}

const char *emmc_result_str(emmc_result_t result)
{
    switch (result) {
    case EMMC_RESULT_OK:
        return "OK";
    case EMMC_RESULT_INVALID_ARG:
        return "Invalid argument";
    case EMMC_RESULT_NOT_INITIALISED:
        return "Controller not initialised";
    case EMMC_RESULT_HW_ERROR:
        return "Hardware error";
    case EMMC_RESULT_OUT_OF_RANGE:
        return "Address out of range";
    default:
        return "Unknown";
    }
}
