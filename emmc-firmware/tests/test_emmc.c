#include "emmc_controller.h"
#include "emmc_hw_if.h"

#include <stdio.h>
#include <string.h>

static int test_write_and_read_back(void)
{
    emmc_controller_t controller = {0};
    emmc_controller_config_t cfg = {
        .clock_hz = 50U * 1000U * 1000U,
        .bus_width_bits = 8U,
        .capacity_blocks = 0U,
    };

    if (emmc_controller_init(&controller, &cfg) != EMMC_RESULT_OK) {
        printf("[FAIL] initialise controller\n");
        return -1;
    }

    unsigned char tx[EMMC_BLOCK_SIZE_BYTES];
    unsigned char rx[EMMC_BLOCK_SIZE_BYTES];
    for (size_t i = 0; i < sizeof(tx); ++i) {
        tx[i] = (unsigned char)(i * 3U);
        rx[i] = 0U;
    }

    if (emmc_controller_write_blocks(&controller, 4U, tx, 1U) != EMMC_RESULT_OK) {
        printf("[FAIL] write block\n");
        return -1;
    }

    if (emmc_controller_read_blocks(&controller, 4U, rx, 1U) != EMMC_RESULT_OK) {
        printf("[FAIL] read block\n");
        return -1;
    }

    if (memcmp(tx, rx, sizeof(tx)) != 0) {
        printf("[FAIL] data mismatch\n");
        return -1;
    }

    printf("[PASS] write and read back\n");
    return 0;
}

static int test_out_of_range_access(void)
{
    emmc_hw_reset();

    emmc_controller_t controller = {0};
    if (emmc_controller_init(&controller, NULL) != EMMC_RESULT_OK) {
        printf("[FAIL] initialise controller\n");
        return -1;
    }

    size_t capacity = controller.config.capacity_blocks;
    unsigned char buffer[EMMC_BLOCK_SIZE_BYTES];
    memset(buffer, 0xA5, sizeof(buffer));

    emmc_result_t res = emmc_controller_write_blocks(&controller, (uint32_t)capacity, buffer, 1U);
    if (res != EMMC_RESULT_OUT_OF_RANGE) {
        printf("[FAIL] expected out of range, got %s\n", emmc_result_str(res));
        return -1;
    }

    printf("[PASS] out of range write rejected\n");
    return 0;
}

int main(void)
{
    int failed = 0;
    failed += (test_write_and_read_back() != 0);
    failed += (test_out_of_range_access() != 0);

    if (failed != 0) {
        printf("eMMC tests failed\n");
        return 1;
    }

    printf("All eMMC tests passed\n");
    return 0;
}
