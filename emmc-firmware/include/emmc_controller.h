#ifndef EMMC_CONTROLLER_H
#define EMMC_CONTROLLER_H

#include <stddef.h>
#include <stdint.h>

#include "emmc_regs.h"

#define EMMC_MAX_BLOCKS_DEFAULT   (1024U)

typedef enum {
    EMMC_RESULT_OK = 0,
    EMMC_RESULT_INVALID_ARG = -1,
    EMMC_RESULT_NOT_INITIALISED = -2,
    EMMC_RESULT_HW_ERROR = -3,
    EMMC_RESULT_OUT_OF_RANGE = -4,
} emmc_result_t;

typedef struct {
    uint32_t clock_hz;
    uint8_t bus_width_bits;
    size_t capacity_blocks;
} emmc_controller_config_t;

typedef struct {
    emmc_controller_config_t config;
    int initialised;
} emmc_controller_t;

emmc_result_t emmc_controller_init(emmc_controller_t *controller,
                                   const emmc_controller_config_t *config);
emmc_result_t emmc_controller_read_blocks(emmc_controller_t *controller,
                                          uint32_t lba,
                                          void *buffer,
                                          size_t block_count);
emmc_result_t emmc_controller_write_blocks(emmc_controller_t *controller,
                                           uint32_t lba,
                                           const void *buffer,
                                           size_t block_count);
const char *emmc_result_str(emmc_result_t result);

#endif /* EMMC_CONTROLLER_H */
