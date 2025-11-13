#ifndef EMMC_FIRMWARE_SIM_EMMC_REGISTERS_H_
#define EMMC_FIRMWARE_SIM_EMMC_REGISTERS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "emmc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32_t regs[EMMC_REG_COUNT];
  uint32_t fifo_buffer[128];  // 模拟 512B 缓冲
  size_t fifo_head;
  size_t fifo_tail;
  size_t fifo_count;
  size_t block_size_bytes;
  bool card_inserted;
} sim_emmc_registers_t;

// 初始化仿真寄存器，复位所有状态
void sim_emmc_registers_init(void);
void sim_emmc_registers_reset(void);
sim_emmc_registers_t *sim_emmc_registers_instance(void);

// 仿真帮助函数
void sim_emmc_set_response(uint32_t resp0, uint32_t resp1,
                           uint32_t resp2, uint32_t resp3);
void sim_emmc_set_status(uint32_t status_mask, bool set);
void sim_emmc_push_fifo_word(uint32_t value);
bool sim_emmc_pop_fifo_word(uint32_t *value);

// 卡片基础属性（仿真）
void sim_emmc_insert_card(uint32_t rca, uint32_t capacity_blocks);
void sim_emmc_remove_card(void);
uint32_t sim_emmc_get_rca(void);
uint32_t sim_emmc_get_capacity_blocks(void);

#ifdef __cplusplus
}
#endif

#endif  // EMMC_FIRMWARE_SIM_EMMC_REGISTERS_H_
