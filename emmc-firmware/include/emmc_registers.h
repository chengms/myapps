/**
 * @file emmc_registers.h
 * @brief eMMC寄存器定义和模拟
 * @description 定义eMMC控制器的所有寄存器位置和功能
 */

#ifndef EMMC_REGISTERS_H
#define EMMC_REGISTERS_H

#include <stdint.h>

/* eMMC控制器基地址（模拟） */
#define EMMC_BASE_ADDR           0x40000000

/* ==================== 寄存器偏移地址 ==================== */

/* 控制寄存器组 */
#define EMMC_REG_CONTROL         0x00    /* 控制寄存器 */
#define EMMC_REG_STATUS          0x04    /* 状态寄存器 */
#define EMMC_REG_INT_ENABLE      0x08    /* 中断使能寄存器 */
#define EMMC_REG_INT_STATUS      0x0C    /* 中断状态寄存器 */
#define EMMC_REG_CLOCK_CONTROL   0x10    /* 时钟控制寄存器 */
#define EMMC_REG_TIMEOUT         0x14    /* 超时寄存器 */

/* 命令寄存器组 */
#define EMMC_REG_CMD_ARG         0x18    /* 命令参数寄存器 */
#define EMMC_REG_CMD             0x1C    /* 命令寄存器 */
#define EMMC_REG_RESP0           0x20    /* 响应寄存器0 */
#define EMMC_REG_RESP1           0x24    /* 响应寄存器1 */
#define EMMC_REG_RESP2           0x28    /* 响应寄存器2 */
#define EMMC_REG_RESP3           0x2C    /* 响应寄存器3 */

/* 数据传输寄存器组 */
#define EMMC_REG_DATA_TIMEOUT    0x30    /* 数据超时寄存器 */
#define EMMC_REG_DATA_LENGTH     0x34    /* 数据长度寄存器 */
#define EMMC_REG_DATA_CONTROL    0x38    /* 数据控制寄存器 */
#define EMMC_REG_DATA_ADDR       0x3C    /* 数据地址寄存器 */
#define EMMC_REG_FIFO            0x40    /* FIFO数据寄存器 */

/* 块大小和计数寄存器 */
#define EMMC_REG_BLOCK_SIZE      0x44    /* 块大小寄存器 */
#define EMMC_REG_BLOCK_COUNT     0x48    /* 块计数寄存器 */

/* 电源和电压寄存器 */
#define EMMC_REG_POWER_CONTROL   0x4C    /* 电源控制寄存器 */
#define EMMC_REG_VOLTAGE_SELECT  0x50    /* 电压选择寄存器 */

/* 版本和特性寄存器 */
#define EMMC_REG_VERSION         0x54    /* 版本寄存器 */
#define EMMC_REG_CAPABILITIES    0x58    /* 能力寄存器 */

/* ==================== 控制寄存器位定义 ==================== */
/* EMMC_REG_CONTROL (0x00) */
#define EMMC_CTRL_RESET          (1 << 0)   /* 软件复位 */
#define EMMC_CTRL_INT_ENABLE     (1 << 1)   /* 中断使能 */
#define EMMC_CTRL_DMA_ENABLE     (1 << 2)   /* DMA使能 */
#define EMMC_CTRL_READ_WAIT      (1 << 3)   /* 读等待 */
#define EMMC_CTRL_SEND_IRQ_RESP  (1 << 4)   /* 发送IRQ响应 */
#define EMMC_CTRL_ABORT_READ     (1 << 5)   /* 中止读操作 */
#define EMMC_CTRL_SEND_CCSD      (1 << 6)   /* 发送CCSD */
#define EMMC_CTRL_SEND_AUTO_STOP (1 << 7)   /* 发送自动停止 */
#define EMMC_CTRL_4BIT_MODE      (1 << 8)   /* 4位数据总线模式 */
#define EMMC_CTRL_8BIT_MODE      (1 << 9)   /* 8位数据总线模式 */
#define EMMC_CTRL_HIGH_SPEED     (1 << 10)  /* 高速模式 */
#define EMMC_CTRL_DDR_MODE       (1 << 11)  /* DDR模式 */

/* ==================== 状态寄存器位定义 ==================== */
/* EMMC_REG_STATUS (0x04) */
#define EMMC_STAT_CMD_COMPLETE   (1 << 0)   /* 命令完成 */
#define EMMC_STAT_DATA_COMPLETE  (1 << 1)   /* 数据完成 */
#define EMMC_STAT_BLOCK_GAP      (1 << 2)   /* 块间隙 */
#define EMMC_STAT_TRANSFER_COMPLETE (1 << 3) /* 传输完成 */
#define EMMC_STAT_WRITE_PROTECT  (1 << 4)   /* 写保护 */
#define EMMC_STAT_INSERTED       (1 << 5)   /* 卡插入 */
#define EMMC_STAT_REMOVED        (1 << 6)   /* 卡移除 */
#define EMMC_STAT_CARD_PRESENT   (1 << 7)   /* 卡存在 */
#define EMMC_STAT_CMD_ACTIVE     (1 << 8)   /* 命令激活 */
#define EMMC_STAT_DATA_ACTIVE    (1 << 9)   /* 数据激活 */
#define EMMC_STAT_FIFO_EMPTY     (1 << 10)  /* FIFO空 */
#define EMMC_STAT_FIFO_FULL      (1 << 11)  /* FIFO满 */
#define EMMC_STAT_BUSY           (1 << 12)  /* 忙状态 */
#define EMMC_STAT_ERROR          (1 << 13)  /* 错误状态 */

/* ==================== 中断寄存器位定义 ==================== */
/* EMMC_REG_INT_ENABLE / EMMC_REG_INT_STATUS */
#define EMMC_INT_CMD_COMPLETE    (1 << 0)   /* 命令完成中断 */
#define EMMC_INT_DATA_COMPLETE   (1 << 1)   /* 数据完成中断 */
#define EMMC_INT_BLOCK_GAP       (1 << 2)   /* 块间隙中断 */
#define EMMC_INT_DMA_COMPLETE    (1 << 3)   /* DMA完成中断 */
#define EMMC_INT_BUF_WRITE_READY (1 << 4)   /* 缓冲区写就绪 */
#define EMMC_INT_BUF_READ_READY  (1 << 5)   /* 缓冲区读就绪 */
#define EMMC_INT_CARD_INSERT     (1 << 6)   /* 卡插入中断 */
#define EMMC_INT_CARD_REMOVE     (1 << 7)   /* 卡移除中断 */
#define EMMC_INT_ERROR           (1 << 8)   /* 错误中断 */

/* ==================== 命令寄存器位定义 ==================== */
/* EMMC_REG_CMD (0x1C) */
#define EMMC_CMD_INDEX_MASK      0x3F       /* 命令索引掩码 (位0-5) */
#define EMMC_CMD_RESPONSE        (1 << 6)   /* 需要响应 */
#define EMMC_CMD_LONG_RESPONSE   (1 << 7)   /* 长响应 */
#define EMMC_CMD_CHECK_RESP_CRC  (1 << 8)   /* 检查响应CRC */
#define EMMC_CMD_DATA_PRESENT    (1 << 9)   /* 数据存在 */
#define EMMC_CMD_READ            (1 << 10)  /* 读操作 */
#define EMMC_CMD_WRITE           (1 << 11)  /* 写操作 */
#define EMMC_CMD_STREAM          (1 << 12)  /* 流模式 */
#define EMMC_CMD_MULTI_BLOCK     (1 << 13)  /* 多块操作 */
#define EMMC_CMD_STOP_TRANSFER   (1 << 14)  /* 停止传输 */
#define EMMC_CMD_WAIT_PRVDATA    (1 << 15)  /* 等待前一个数据 */
#define EMMC_CMD_SEND_AUTO_STOP  (1 << 16)  /* 发送自动停止 */
#define EMMC_CMD_ABORT_CMD       (1 << 17)  /* 中止命令 */

/* ==================== 数据控制寄存器位定义 ==================== */
/* EMMC_REG_DATA_CONTROL (0x38) */
#define EMMC_DATA_CTRL_ENABLE    (1 << 0)   /* 数据使能 */
#define EMMC_DATA_CTRL_DIRECTION (1 << 1)   /* 方向：0=读，1=写 */
#define EMMC_DATA_CTRL_MODE      (1 << 2)   /* 模式：0=块，1=流 */
#define EMMC_DATA_CTRL_DMA_ENABLE (1 << 3)  /* DMA使能 */

/* ==================== 时钟控制寄存器位定义 ==================== */
/* EMMC_REG_CLOCK_CONTROL (0x10) */
#define EMMC_CLK_ENABLE          (1 << 0)   /* 时钟使能 */
#define EMMC_CLK_DIVIDER_MASK    0xFF00     /* 分频器掩码 (位8-15) */
#define EMMC_CLK_DIVIDER_SHIFT   8          /* 分频器移位 */
#define EMMC_CLK_SELECT_MASK     0x03       /* 时钟源选择 (位16-17) */
#define EMMC_CLK_SELECT_SHIFT    16

/* ==================== 电源控制寄存器位定义 ==================== */
/* EMMC_REG_POWER_CONTROL (0x4C) */
#define EMMC_PWR_ON              (1 << 0)   /* 电源开启 */
#define EMMC_PWR_VOLTAGE_MASK    0x0E       /* 电压选择掩码 (位1-3) */
#define EMMC_PWR_VOLTAGE_SHIFT   1

/* ==================== eMMC命令定义 ==================== */
/* 基本命令 (CMD0-15) */
#define EMMC_CMD_GO_IDLE_STATE           0
#define EMMC_CMD_SEND_OP_COND            1
#define EMMC_CMD_ALL_SEND_CID            2
#define EMMC_CMD_SEND_RELATIVE_ADDR      3
#define EMMC_CMD_SET_DSR                 4
#define EMMC_CMD_SWITCH                  6
#define EMMC_CMD_SELECT_CARD             7
#define EMMC_CMD_SEND_EXT_CSD            8
#define EMMC_CMD_SEND_CSD                9
#define EMMC_CMD_SEND_CID                10
#define EMMC_CMD_STOP_TRANSMISSION       12
#define EMMC_CMD_SEND_STATUS             13
#define EMMC_CMD_GO_INACTIVE_STATE       15

/* 读命令 */
#define EMMC_CMD_READ_SINGLE_BLOCK       17
#define EMMC_CMD_READ_MULTIPLE_BLOCK     18

/* 写命令 */
#define EMMC_CMD_WRITE_BLOCK             24
#define EMMC_CMD_WRITE_MULTIPLE_BLOCK    25

/* 擦除命令 */
#define EMMC_CMD_ERASE_GROUP_START       35
#define EMMC_CMD_ERASE_GROUP_END         36
#define EMMC_CMD_ERASE                   38

/* 应用特定命令 */
#define EMMC_CMD_APP_CMD                 55
#define EMMC_CMD_APP_SET_BUS_WIDTH       6   /* ACMD6 */
#define EMMC_CMD_APP_SD_STATUS           13  /* ACMD13 */
#define EMMC_CMD_APP_SEND_OP_COND       41  /* ACMD41 */

/* ==================== 寄存器访问宏定义 ==================== */
#define EMMC_REG(offset)         (*((volatile uint32_t *)(EMMC_BASE_ADDR + (offset))))

/* 寄存器读写宏 */
#define EMMC_READ_REG(offset)    EMMC_REG(offset)
#define EMMC_WRITE_REG(offset, value) do { EMMC_REG(offset) = (value); } while(0)

/* 位操作宏 */
#define EMMC_SET_BITS(offset, bits)   do { EMMC_REG(offset) |= (bits); } while(0)
#define EMMC_CLEAR_BITS(offset, bits) do { EMMC_REG(offset) &= ~(bits); } while(0)
#define EMMC_GET_BITS(offset, mask)   (EMMC_REG(offset) & (mask))

/* ==================== 响应类型定义 ==================== */
typedef enum {
    EMMC_RESPONSE_NONE = 0,      /* 无响应 */
    EMMC_RESPONSE_R1,            /* 正常响应 */
    EMMC_RESPONSE_R1B,           /* 带忙信号的响应 */
    EMMC_RESPONSE_R2,            /* CID/CSD响应 */
    EMMC_RESPONSE_R3,            /* OCR响应 */
    EMMC_RESPONSE_R4,            /* 未使用 */
    EMMC_RESPONSE_R5,            /* 未使用 */
    EMMC_RESPONSE_R6            /* 发布RCA响应 */
} emmc_response_type_t;

/* ==================== 错误代码定义 ==================== */
typedef enum {
    EMMC_OK = 0,
    EMMC_ERROR_TIMEOUT,
    EMMC_ERROR_CRC,
    EMMC_ERROR_CMD,
    EMMC_ERROR_DATA,
    EMMC_ERROR_BUSY,
    EMMC_ERROR_NOT_PRESENT,
    EMMC_ERROR_INVALID_PARAM,
    EMMC_ERROR_NOT_INITIALIZED
} emmc_error_t;

#endif /* EMMC_REGISTERS_H */
