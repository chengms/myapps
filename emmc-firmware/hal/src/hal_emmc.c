#include "hal_emmc.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "emmc_registers.h"

#define EMMC_SIM_BLOCK_SIZE_BYTES 512U
#define EMMC_SIM_MAX_BLOCKS       4096U

typedef struct {
  bool expect_write;
  bool expect_read;
  uint32_t pending_lba;
  size_t transfer_bytes;
  size_t transferred_bytes;
  uint32_t last_cmd;
  uint32_t last_arg;
} hal_sim_state_t;

static hal_sim_state_t g_state;
static uint8_t g_card_storage[EMMC_SIM_BLOCK_SIZE_BYTES * EMMC_SIM_MAX_BLOCKS];

static void hal_sim_state_reset(void) {
  memset(&g_state, 0, sizeof(g_state));
}

static void hal_sim_fill_default_storage(void) {
  for (size_t block = 0; block < EMMC_SIM_MAX_BLOCKS; ++block) {
    for (size_t i = 0; i < EMMC_SIM_BLOCK_SIZE_BYTES; ++i) {
      g_card_storage[block * EMMC_SIM_BLOCK_SIZE_BYTES + i] =
          (uint8_t)((block + i) & 0xFFU);
    }
  }
}

static void hal_sim_handle_cmd(uint32_t cmd_value);

bool hal_emmc_init(void) {
  sim_emmc_registers_init();
  hal_sim_state_reset();
  hal_sim_fill_default_storage();
  return true;
}

void hal_emmc_reset(void) {
  sim_emmc_registers_reset();
  hal_sim_state_reset();
}

uint32_t hal_emmc_reg_read(emmc_register_t reg) {
  return sim_emmc_registers_instance()->regs[reg];
}

void hal_emmc_reg_write(emmc_register_t reg, uint32_t value) {
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  switch (reg) {
    case EMMC_REG_INT_STATUS:
      regs->regs[EMMC_REG_INT_STATUS] &= ~value;
      break;
    case EMMC_REG_DATA_LENGTH:
      regs->block_size_bytes = value;
      regs->regs[EMMC_REG_DATA_LENGTH] = value;
      break;
    case EMMC_REG_DATA_CTRL:
      regs->regs[reg] = value;
      if (value & EMMC_DATA_CTRL_ENABLE) {
        g_state.transfer_bytes = regs->block_size_bytes;
        g_state.transferred_bytes = 0U;
      }
      break;
    case EMMC_REG_FIFO:
      sim_emmc_push_fifo_word(value);
      break;
    case EMMC_REG_CMD_TRANSFER:
      regs->regs[EMMC_REG_CMD_TRANSFER] = value;
      hal_sim_handle_cmd(value);
      break;
    case EMMC_REG_SYS_CTRL:
      regs->regs[reg] = value;
      if (value & EMMC_SYS_CTRL_RST) {
        hal_emmc_reset();
        regs->regs[EMMC_REG_SYS_CTRL] &= ~EMMC_SYS_CTRL_RST;
        regs->regs[EMMC_REG_SYS_CTRL] |= EMMC_SYS_CTRL_RST_DONE;
      }
      break;
    default:
      regs->regs[reg] = value;
      break;
  }
}

void hal_emmc_delay_us(uint32_t microseconds) {
  (void)microseconds;
}

void hal_emmc_log(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  fputc('\n', stdout);
  va_end(args);
}

uint32_t hal_emmc_fifo_read(void) {
  uint32_t value = 0U;
  if (!sim_emmc_pop_fifo_word(&value)) {
    return 0U;
  }
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  regs->regs[EMMC_REG_STATUS] &= ~EMMC_STATUS_FIFO_FULL;
  if (g_state.expect_read) {
    g_state.transferred_bytes += sizeof(uint32_t);
    if (g_state.transferred_bytes >= g_state.transfer_bytes) {
      g_state.expect_read = false;
      regs->regs[EMMC_REG_INT_STATUS] |= EMMC_INT_DATA_DONE;
    }
  }
  return value;
}

void hal_emmc_fifo_write(uint32_t value) {
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  if (g_state.expect_write) {
    size_t offset = g_state.transferred_bytes;
    if (offset < g_state.transfer_bytes) {
      size_t base = g_state.pending_lba * EMMC_SIM_BLOCK_SIZE_BYTES + offset;
      if (offset + sizeof(uint32_t) <= g_state.transfer_bytes) {
        g_card_storage[base + 0] = (uint8_t)(value & 0xFFU);
        g_card_storage[base + 1] = (uint8_t)((value >> 8) & 0xFFU);
        g_card_storage[base + 2] = (uint8_t)((value >> 16) & 0xFFU);
        g_card_storage[base + 3] = (uint8_t)((value >> 24) & 0xFFU);
      }
      g_state.transferred_bytes += sizeof(uint32_t);
      if (g_state.transferred_bytes >= g_state.transfer_bytes) {
        g_state.expect_write = false;
        regs->regs[EMMC_REG_INT_STATUS] |= EMMC_INT_DATA_DONE;
        hal_emmc_fifo_flush();
      }
    }
  } else {
    sim_emmc_push_fifo_word(value);
  }
}

void hal_emmc_fifo_flush(void) {
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  while (sim_emmc_pop_fifo_word(NULL)) {
  }
  regs->regs[EMMC_REG_STATUS] |= EMMC_STATUS_FIFO_EMPTY;
}

static void hal_sim_set_cmd_done(void) {
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  regs->regs[EMMC_REG_STATUS] &= ~(EMMC_STATUS_CMD_INHIBIT);
  regs->regs[EMMC_REG_INT_STATUS] |= EMMC_INT_CMD_DONE;
}

static void hal_sim_prepare_read(uint32_t lba) {
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  size_t bytes = regs->block_size_bytes;
  if (bytes == 0U) {
    bytes = EMMC_SIM_BLOCK_SIZE_BYTES;
    regs->block_size_bytes = bytes;
  }
  if (lba >= EMMC_SIM_MAX_BLOCKS) {
    hal_emmc_log("[SIM] 读取越界 LBA=%u", lba);
    return;
  }
  hal_emmc_fifo_flush();
  const uint8_t *src = &g_card_storage[lba * EMMC_SIM_BLOCK_SIZE_BYTES];
  for (size_t i = 0; i < bytes; i += sizeof(uint32_t)) {
    uint32_t word = src[i] | ((uint32_t)src[i + 1] << 8) |
                    ((uint32_t)src[i + 2] << 16) |
                    ((uint32_t)src[i + 3] << 24);
    sim_emmc_push_fifo_word(word);
  }
  g_state.expect_read = true;
  g_state.transfer_bytes = bytes;
  g_state.transferred_bytes = 0U;
  g_state.pending_lba = lba;
  sim_emmc_set_status(EMMC_STATUS_FIFO_EMPTY, false);
  sim_emmc_set_status(EMMC_STATUS_FIFO_FULL, false);
}

static void hal_sim_prepare_write(uint32_t lba) {
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  size_t bytes = regs->block_size_bytes;
  if (bytes == 0U) {
    bytes = EMMC_SIM_BLOCK_SIZE_BYTES;
    regs->block_size_bytes = bytes;
  }
  if (lba >= EMMC_SIM_MAX_BLOCKS) {
    hal_emmc_log("[SIM] 写入越界 LBA=%u", lba);
    return;
  }
  g_state.expect_write = true;
  g_state.transfer_bytes = bytes;
  g_state.transferred_bytes = 0U;
  g_state.pending_lba = lba;
  hal_emmc_fifo_flush();
}

static void hal_sim_handle_cmd(uint32_t cmd_value) {
  uint8_t cmd_index = (uint8_t)(cmd_value & EMMC_CMD_INDEX_MASK);
  g_state.last_cmd = cmd_index;
  sim_emmc_registers_t *regs = sim_emmc_registers_instance();
  switch (cmd_index) {
    case 0:  // CMD0: GO_IDLE_STATE
      sim_emmc_registers_reset();
      sim_emmc_registers_init();
      hal_sim_set_cmd_done();
      break;
    case 1:  // CMD1: SEND_OP_COND
      sim_emmc_set_response(0x80FF8000U, 0, 0, 0);
      hal_sim_set_cmd_done();
      break;
    case 2:  // CMD2: ALL_SEND_CID
      sim_emmc_set_response(0x12345678U, 0x9ABCDEF0U, 0x0FEDCBA9U, 0x87654321U);
      hal_sim_set_cmd_done();
      break;
    case 3:  // CMD3: SET_RELATIVE_ADDR
      sim_emmc_set_response(sim_emmc_get_rca() << 16, 0, 0, 0);
      hal_sim_set_cmd_done();
      break;
    case 7:  // CMD7: SELECT/DESELECT_CARD
      hal_sim_set_cmd_done();
      break;
    case 8:  // CMD8: SEND_IF_COND
      sim_emmc_set_response(0x000001AAU, 0, 0, 0);
      hal_sim_set_cmd_done();
      break;
    case 9:  // CMD9: SEND_CSD
      sim_emmc_set_response(0x400E0032U, 0x5B590000U, 0xEDCBA987U, 0x00000000U);
      hal_sim_set_cmd_done();
      break;
    case 13:  // CMD13: SEND_STATUS
      sim_emmc_set_response(0x00000900U, 0, 0, 0);
      hal_sim_set_cmd_done();
      break;
    case 17:  // CMD17: READ_SINGLE_BLOCK
      hal_sim_set_cmd_done();
      hal_sim_prepare_read(regs->regs[EMMC_REG_CMD_ARG]);
      break;
    case 24:  // CMD24: WRITE_BLOCK
      hal_sim_set_cmd_done();
      hal_sim_prepare_write(regs->regs[EMMC_REG_CMD_ARG]);
      break;
    default:
      hal_emmc_log("[SIM] 未实现的命令 CMD%u", cmd_index);
      regs->regs[EMMC_REG_INT_STATUS] |= EMMC_INT_ERROR;
      hal_sim_set_cmd_done();
      break;
  }
}
