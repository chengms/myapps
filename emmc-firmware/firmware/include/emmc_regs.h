#ifndef EMMC_FIRMWARE_INCLUDE_EMMC_REGS_H_
#define EMMC_FIRMWARE_INCLUDE_EMMC_REGS_H_

#include <stdint.h>

// 模拟的 eMMC 主控制器寄存器索引
// 可根据实际芯片文档映射至真实地址

typedef enum {
  EMMC_REG_SYS_CTRL = 0,      // 系统控制寄存器（复位、复位完成）
  EMMC_REG_CLK_CTRL,          // 时钟控制寄存器
  EMMC_REG_PWR_CTRL,          // 电源控制寄存器
  EMMC_REG_HOST_CTRL,         // 主机配置寄存器（总线宽度、速度模式）
  EMMC_REG_INT_STATUS,        // 中断状态寄存器
  EMMC_REG_INT_ENABLE,        // 中断使能寄存器
  EMMC_REG_CMD_ARG,           // 命令参数寄存器
  EMMC_REG_CMD_TRANSFER,      // 命令与传输控制寄存器
  EMMC_REG_RESP0,             // 响应寄存器0（R1/R3/R6）
  EMMC_REG_RESP1,
  EMMC_REG_RESP2,
  EMMC_REG_RESP3,
  EMMC_REG_DATA_TIMER,        // 数据超时控制
  EMMC_REG_DATA_LENGTH,       // 数据长度
  EMMC_REG_DATA_CTRL,         // 数据控制（读写、块大小）
  EMMC_REG_STATUS,            // 当前状态寄存器
  EMMC_REG_FIFO,              // 数据 FIFO 寄存器
  EMMC_REG_BOOT_CTRL,         // Boot 相关控制
  EMMC_REG_EXT_CSD_CFG,       // EXT_CSD 访问配置
  EMMC_REG_COUNT
} emmc_register_t;

// SYS_CTRL 位定义
#define EMMC_SYS_CTRL_RST       (1u << 0)
#define EMMC_SYS_CTRL_RST_DONE  (1u << 1)

// CLK_CTRL 位定义
#define EMMC_CLK_CTRL_ENABLE    (1u << 0)
#define EMMC_CLK_CTRL_STABLE    (1u << 1)

// PWR_CTRL 位定义
#define EMMC_PWR_CTRL_ON        (1u << 0)

// HOST_CTRL 位定义
#define EMMC_HOST_CTRL_BUS_WIDTH_MASK  (0x3u << 0)
#define EMMC_HOST_CTRL_BUS_WIDTH_1BIT  (0x0u << 0)
#define EMMC_HOST_CTRL_BUS_WIDTH_4BIT  (0x1u << 0)
#define EMMC_HOST_CTRL_BUS_WIDTH_8BIT  (0x2u << 0)
#define EMMC_HOST_CTRL_HIGH_SPEED      (1u << 2)

// CMD_TRANSFER 位定义
#define EMMC_CMD_START          (1u << 31)
#define EMMC_CMD_RESP_EXPECT    (1u << 6)
#define EMMC_CMD_RESP_LONG      (1u << 7)
#define EMMC_CMD_DATA_PRESENT   (1u << 8)
#define EMMC_CMD_READ           (1u << 9)
#define EMMC_CMD_WRITE          (0u << 9)
#define EMMC_CMD_INDEX_MASK     (0x3Fu)

// DATA_CTRL 位定义
#define EMMC_DATA_CTRL_ENABLE   (1u << 0)
#define EMMC_DATA_CTRL_DIR_READ (1u << 1)
#define EMMC_DATA_CTRL_DMA      (1u << 3)
#define EMMC_DATA_BLOCK_SIZE_SHIFT 4

// STATUS 位定义（仿真使用）
#define EMMC_STATUS_CMD_INHIBIT   (1u << 0)
#define EMMC_STATUS_DATA_INHIBIT  (1u << 1)
#define EMMC_STATUS_FIFO_EMPTY    (1u << 2)
#define EMMC_STATUS_FIFO_FULL     (1u << 3)

// INT_STATUS 位定义
#define EMMC_INT_CMD_DONE       (1u << 0)
#define EMMC_INT_DATA_DONE      (1u << 1)
#define EMMC_INT_ERROR          (1u << 15)

#endif  // EMMC_FIRMWARE_INCLUDE_EMMC_REGS_H_
