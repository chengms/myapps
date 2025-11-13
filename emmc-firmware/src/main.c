#include "emmc_controller.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    emmc_controller_t controller = {0};
    emmc_controller_config_t config = {
        .clock_hz = 26U * 1000U * 1000U,
        .bus_width_bits = 4U,
        .capacity_blocks = 0U,
    };

    emmc_result_t init_res = emmc_controller_init(&controller, &config);
    if (init_res != EMMC_RESULT_OK) {
        fprintf(stderr, "Init failed: %s\n", emmc_result_str(init_res));
        return 1;
    }

    unsigned char tx_buffer[EMMC_BLOCK_SIZE_BYTES] = {0};
    unsigned char rx_buffer[EMMC_BLOCK_SIZE_BYTES] = {0};

    for (size_t i = 0; i < sizeof(tx_buffer); ++i) {
        tx_buffer[i] = (unsigned char)(i & 0xFFU);
    }

    emmc_result_t wr = emmc_controller_write_blocks(&controller, 0U, tx_buffer, 1U);
    if (wr != EMMC_RESULT_OK) {
        fprintf(stderr, "Write failed: %s\n", emmc_result_str(wr));
        return 1;
    }

    emmc_result_t rd = emmc_controller_read_blocks(&controller, 0U, rx_buffer, 1U);
    if (rd != EMMC_RESULT_OK) {
        fprintf(stderr, "Read failed: %s\n", emmc_result_str(rd));
        return 1;
    }

    if (memcmp(tx_buffer, rx_buffer, sizeof(tx_buffer)) != 0) {
        fprintf(stderr, "Data mismatch after read-back\n");
        return 1;
    }

    printf("eMMC simulation succeeded. First byte: 0x%02X\n", rx_buffer[0]);
    return 0;
}
