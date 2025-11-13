#include "emmc_registers.h"

#include <string.h>

static sim_emmc_registers_t g_regs;
static uint32_t g_card_rca = 0x1U;
static uint32_t g_card_capacity_blocks = 4096U;  // 默认 2MB (4096 * 512B)

static void sim_emmc_fifo_reset(void) {
  g_regs.fifo_head = 0U;
  g_regs.fifo_tail = 0U;
  g_regs.fifo_count = 0U;
  g_regs.block_size_bytes = 512U;
  memset(g_regs.fifo_buffer, 0, sizeof(g_regs.fifo_buffer));
  sim_emmc_set_status(EMMC_STATUS_FIFO_EMPTY, true);
  sim_emmc_set_status(EMMC_STATUS_FIFO_FULL, false);
}

void sim_emmc_registers_init(void) {
  memset(&g_regs, 0, sizeof(g_regs));
  sim_emmc_fifo_reset();
  g_regs.card_inserted = true;
  g_regs.regs[EMMC_REG_STATUS] = EMMC_STATUS_FIFO_EMPTY;
  g_regs.regs[EMMC_REG_PWR_CTRL] = EMMC_PWR_CTRL_ON;
  g_regs.regs[EMMC_REG_CLK_CTRL] = EMMC_CLK_CTRL_STABLE;
  g_regs.regs[EMMC_REG_SYS_CTRL] = EMMC_SYS_CTRL_RST_DONE;
}

void sim_emmc_registers_reset(void) {
  sim_emmc_registers_init();
}

sim_emmc_registers_t *sim_emmc_registers_instance(void) {
  return &g_regs;
}

void sim_emmc_set_response(uint32_t resp0, uint32_t resp1,
                           uint32_t resp2, uint32_t resp3) {
  g_regs.regs[EMMC_REG_RESP0] = resp0;
  g_regs.regs[EMMC_REG_RESP1] = resp1;
  g_regs.regs[EMMC_REG_RESP2] = resp2;
  g_regs.regs[EMMC_REG_RESP3] = resp3;
}

void sim_emmc_set_status(uint32_t status_mask, bool set) {
  if (set) {
    g_regs.regs[EMMC_REG_STATUS] |= status_mask;
  } else {
    g_regs.regs[EMMC_REG_STATUS] &= ~status_mask;
  }
}

void sim_emmc_push_fifo_word(uint32_t value) {
  if (g_regs.fifo_count >= sizeof(g_regs.fifo_buffer) / sizeof(g_regs.fifo_buffer[0])) {
    sim_emmc_set_status(EMMC_STATUS_FIFO_FULL, true);
    return;
  }
  g_regs.fifo_buffer[g_regs.fifo_head] = value;
  g_regs.fifo_head = (g_regs.fifo_head + 1U) % (sizeof(g_regs.fifo_buffer) / sizeof(g_regs.fifo_buffer[0]));
  g_regs.fifo_count++;
  sim_emmc_set_status(EMMC_STATUS_FIFO_EMPTY, g_regs.fifo_count == 0U);
  sim_emmc_set_status(EMMC_STATUS_FIFO_FULL,
                      g_regs.fifo_count == sizeof(g_regs.fifo_buffer) /
                                             sizeof(g_regs.fifo_buffer[0]));
}

bool sim_emmc_pop_fifo_word(uint32_t *value) {
  if (g_regs.fifo_count == 0U) {
    sim_emmc_set_status(EMMC_STATUS_FIFO_EMPTY, true);
    return false;
  }
  if (value != NULL) {
    *value = g_regs.fifo_buffer[g_regs.fifo_tail];
  }
  g_regs.fifo_tail = (g_regs.fifo_tail + 1U) % (sizeof(g_regs.fifo_buffer) / sizeof(g_regs.fifo_buffer[0]));
  g_regs.fifo_count--;
  sim_emmc_set_status(EMMC_STATUS_FIFO_FULL, false);
  sim_emmc_set_status(EMMC_STATUS_FIFO_EMPTY, g_regs.fifo_count == 0U);
  return true;
}

void sim_emmc_insert_card(uint32_t rca, uint32_t capacity_blocks) {
  g_regs.card_inserted = true;
  g_card_rca = rca;
  if (capacity_blocks != 0U) {
    g_card_capacity_blocks = capacity_blocks;
  }
}

void sim_emmc_remove_card(void) {
  g_regs.card_inserted = false;
}

uint32_t sim_emmc_get_rca(void) {
  return g_card_rca;
}

uint32_t sim_emmc_get_capacity_blocks(void) {
  return g_card_capacity_blocks;
}
