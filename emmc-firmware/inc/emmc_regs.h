/**
 * @file emmc_regs.h
 * @brief eMMC控制器寄存器定义（模拟）
 * @version 1.0
 * @date 2025-11-13
 * 
 * 本文件定义了eMMC控制器的所有寄存器映射
 * 基于标准JEDEC eMMC 5.1规范的控制器实现
 */

#ifndef __EMMC_REGS_H__
#define __EMMC_REGS_H__

#include <stdint.h>

/* eMMC控制器基地址（模拟） */
#define EMMC_BASE_ADDR          0x10000000

/* 系统寄存器偏移 */
#define EMMC_SYSADDR            0x000   /* DMA系统地址 */
#define EMMC_BLKSIZE            0x004   /* 块大小 */
#define EMMC_BLKCNT             0x006   /* 块计数 */
#define EMMC_ARGUMENT           0x008   /* 命令参数 */
#define EMMC_XFER_MODE          0x00C   /* 传输模式 */
#define EMMC_COMMAND            0x00E   /* 命令 */
#define EMMC_RESPONSE0          0x010   /* 响应寄存器0 */
#define EMMC_RESPONSE1          0x014   /* 响应寄存器1 */
#define EMMC_RESPONSE2          0x018   /* 响应寄存器2 */
#define EMMC_RESPONSE3          0x01C   /* 响应寄存器3 */
#define EMMC_DATA_PORT          0x020   /* 数据端口 */
#define EMMC_PRESENT_STATE      0x024   /* 当前状态 */
#define EMMC_HOST_CTRL          0x028   /* 主机控制 */
#define EMMC_PWR_CTRL           0x029   /* 电源控制 */
#define EMMC_BLOCK_GAP_CTRL     0x02A   /* 块间隙控制 */
#define EMMC_WAKEUP_CTRL        0x02B   /* 唤醒控制 */
#define EMMC_CLOCK_CTRL         0x02C   /* 时钟控制 */
#define EMMC_TIMEOUT_CTRL       0x02E   /* 超时控制 */
#define EMMC_SW_RESET           0x02F   /* 软件复位 */
#define EMMC_INT_STATUS         0x030   /* 中断状态 */
#define EMMC_INT_STATUS_EN      0x034   /* 中断状态使能 */
#define EMMC_INT_SIGNAL_EN      0x038   /* 中断信号使能 */
#define EMMC_ACMD12_ERR         0x03C   /* 自动CMD12错误状态 */
#define EMMC_HOST_CTRL2         0x03E   /* 主机控制2 */
#define EMMC_CAPABILITIES       0x040   /* 能力寄存器 */
#define EMMC_CAPABILITIES_RES   0x044   /* 能力寄存器保留 */
#define EMMC_MAX_CURRENT        0x048   /* 最大电流能力 */
#define EMMC_FORCE_EVENT        0x050   /* 强制事件 */
#define EMMC_ADMA_ERR_STATUS    0x054   /* ADMA错误状态 */
#define EMMC_ADMA_ADDR_LOW      0x058   /* ADMA地址低32位 */
#define EMMC_ADMA_ADDR_HIGH     0x05C   /* ADMA地址高32位 */

/* 供应商特定寄存器 */
#define EMMC_VENDOR_REG         0x100   /* 供应商寄存器 */
#define EMMC_VENDOR_REG2        0x104   /* 供应商寄存器2 */

/* 传输模式寄存器位定义 */
#define EMMC_XFER_DMA_EN        (1 << 0)   /* DMA使能 */
#define EMMC_XFER_BLK_CNT_EN    (1 << 1)   /* 块计数使能 */
#define EMMC_XFER_ACMD12_EN     (1 << 2)   /* 自动CMD12使能 */
#define EMMC_XFER_DATA_DIR      (1 << 4)   /* 数据传输方向(1=读) */
#define EMMC_XFER_MULTI_BLK     (1 << 5)   /* 多块选择 */

/* 命令寄存器位定义 */
#define EMMC_CMD_RESP_NONE      (0 << 0)   /* 无响应 */
#define EMMC_CMD_RESP_136       (1 << 0)   /* 响应长度136位 */
#define EMMC_CMD_RESP_48        (2 << 0)   /* 响应长度48位 */
#define EMMC_CMD_RESP_48_BUSY   (3 << 0)   /* 响应长度48位+忙 */
#define EMMC_CMD_CRC_EN         (1 << 3)   /* CRC检查使能 */
#define EMMC_CMD_IDX_EN         (1 << 4)   /* 命令索引检查使能 */
#define EMMC_CMD_DATA_EN        (1 << 5)   /* 数据存在 */
#define EMMC_CMD_TYPE_NORMAL    (0 << 6)   /* 正常命令 */
#define EMMC_CMD_TYPE_SUSPEND   (1 << 6)   /* 挂起命令 */
#define EMMC_CMD_TYPE_RESUME    (2 << 6)   /* 恢复命令 */
#define EMMC_CMD_TYPE_ABORT     (3 << 6)   /* 中止命令 */

/* 当前状态寄存器位定义 */
#define EMMC_STATE_CMD_INHIBIT  (1 << 0)   /* 命令禁止 */
#define EMMC_STATE_DAT_INHIBIT  (1 << 1)   /* 数据禁止 */
#define EMMC_STATE_DAT_ACTIVE   (1 << 2)   /* 数据线活动 */
#define EMMC_STATE_WR_ACTIVE    (1 << 8)   /* 写传输活动 */
#define EMMC_STATE_RD_ACTIVE    (1 << 9)   /* 读传输活动 */
#define EMMC_STATE_BUF_WR_EN    (1 << 10)  /* 缓冲区写使能 */
#define EMMC_STATE_BUF_RD_EN    (1 << 11)  /* 缓冲区读使能 */
#define EMMC_STATE_CARD_INSERT  (1 << 16)  /* 卡插入 */
#define EMMC_STATE_CARD_STABLE  (1 << 17)  /* 卡状态稳定 */

/* 主机控制寄存器位定义 */
#define EMMC_CTRL_LED_ON        (1 << 0)   /* LED控制 */
#define EMMC_CTRL_DATA_WIDTH    (1 << 1)   /* 数据传输宽度(1=4bit) */
#define EMMC_CTRL_HIGH_SPEED    (1 << 2)   /* 高速使能 */
#define EMMC_CTRL_DMA_MASK      (3 << 3)   /* DMA选择掩码 */
#define EMMC_CTRL_DMA_SDMA      (0 << 3)   /* SDMA */
#define EMMC_CTRL_DMA_ADMA32    (2 << 3)   /* 32位ADMA2 */
#define EMMC_CTRL_DMA_ADMA64    (3 << 3)   /* 64位ADMA2 */

/* 电源控制寄存器位定义 */
#define EMMC_PWR_CTRL_ON        (1 << 0)   /* 电源开启 */
#define EMMC_PWR_VOLTAGE_MASK   (7 << 1)   /* 电压选择掩码 */
#define EMMC_PWR_VOLTAGE_1V8    (5 << 1)   /* 1.8V */
#define EMMC_PWR_VOLTAGE_3V0    (6 << 1)   /* 3.0V */
#define EMMC_PWR_VOLTAGE_3V3    (7 << 1)   /* 3.3V */

/* 时钟控制寄存器位定义 */
#define EMMC_CLK_INT_CLK_EN     (1 << 0)   /* 内部时钟使能 */
#define EMMC_CLK_INT_CLK_STABLE (1 << 1)   /* 内部时钟稳定 */
#define EMMC_CLK_CARD_EN        (1 << 2)   /* SD时钟使能 */
#define EMMC_CLK_DIV_SHIFT      8          /* 时钟分频器位移 */
#define EMMC_CLK_DIV_MASK       0xFF       /* 时钟分频器掩码 */

/* 软件复位寄存器位定义 */
#define EMMC_RESET_ALL          (1 << 0)   /* 复位所有 */
#define EMMC_RESET_CMD          (1 << 1)   /* 复位CMD线 */
#define EMMC_RESET_DATA         (1 << 2)   /* 复位DAT线 */

/* 中断状态寄存器位定义 */
#define EMMC_INT_CMD_DONE       (1 << 0)   /* 命令完成 */
#define EMMC_INT_XFER_DONE      (1 << 1)   /* 传输完成 */
#define EMMC_INT_BLOCK_GAP      (1 << 2)   /* 块间隙事件 */
#define EMMC_INT_DMA            (1 << 3)   /* DMA中断 */
#define EMMC_INT_BUF_WR_READY   (1 << 4)   /* 缓冲区写就绪 */
#define EMMC_INT_BUF_RD_READY   (1 << 5)   /* 缓冲区读就绪 */
#define EMMC_INT_CARD_INSERT    (1 << 6)   /* 卡插入 */
#define EMMC_INT_CARD_REMOVE    (1 << 7)   /* 卡移除 */
#define EMMC_INT_CARD_INT       (1 << 8)   /* 卡中断 */
#define EMMC_INT_ERROR          (1 << 15)  /* 错误中断 */
#define EMMC_INT_TIMEOUT        (1 << 16)  /* 超时错误 */
#define EMMC_INT_CRC_ERROR      (1 << 17)  /* CRC错误 */
#define EMMC_INT_END_BIT_ERROR  (1 << 18)  /* 结束位错误 */
#define EMMC_INT_INDEX_ERROR    (1 << 19)  /* 索引错误 */
#define EMMC_INT_DATA_TIMEOUT   (1 << 20)  /* 数据超时 */
#define EMMC_INT_DATA_CRC_ERROR (1 << 21)  /* 数据CRC错误 */
#define EMMC_INT_DATA_END_ERROR (1 << 22)  /* 数据结束位错误 */
#define EMMC_INT_BUS_POWER      (1 << 23)  /* 总线电源错误 */
#define EMMC_INT_ACMD12_ERROR   (1 << 24)  /* 自动CMD12错误 */
#define EMMC_INT_ADMA_ERROR     (1 << 25)  /* ADMA错误 */

/* eMMC命令定义 */
#define EMMC_CMD0_GO_IDLE           0
#define EMMC_CMD1_SEND_OP_COND      1
#define EMMC_CMD2_ALL_SEND_CID      2
#define EMMC_CMD3_SET_RELATIVE_ADDR 3
#define EMMC_CMD6_SWITCH            6
#define EMMC_CMD7_SELECT_CARD       7
#define EMMC_CMD8_SEND_EXT_CSD      8
#define EMMC_CMD9_SEND_CSD          9
#define EMMC_CMD12_STOP_TRANS       12
#define EMMC_CMD13_SEND_STATUS      13
#define EMMC_CMD16_SET_BLOCKLEN     16
#define EMMC_CMD17_READ_SINGLE      17
#define EMMC_CMD18_READ_MULTIPLE    18
#define EMMC_CMD23_SET_BLOCK_COUNT  23
#define EMMC_CMD24_WRITE_SINGLE     24
#define EMMC_CMD25_WRITE_MULTIPLE   25
#define EMMC_CMD35_ERASE_GROUP_START 35
#define EMMC_CMD36_ERASE_GROUP_END  36
#define EMMC_CMD38_ERASE            38

/* 寄存器访问宏 */
#define EMMC_REG(offset)        (*(volatile uint32_t*)(EMMC_BASE_ADDR + (offset)))
#define EMMC_REG_READ(offset)   EMMC_REG(offset)
#define EMMC_REG_WRITE(offset, val) (EMMC_REG(offset) = (val))
#define EMMC_REG_SET_BITS(offset, bits) (EMMC_REG(offset) |= (bits))
#define EMMC_REG_CLR_BITS(offset, bits) (EMMC_REG(offset) &= ~(bits))

/* eMMC控制器寄存器结构体定义 */
typedef struct {
    volatile uint32_t sysaddr;          /* 0x000 DMA系统地址 */
    volatile uint16_t blksize;          /* 0x004 块大小 */
    volatile uint16_t blkcnt;           /* 0x006 块计数 */
    volatile uint32_t argument;         /* 0x008 命令参数 */
    volatile uint16_t xfer_mode;        /* 0x00C 传输模式 */
    volatile uint16_t command;          /* 0x00E 命令 */
    volatile uint32_t response[4];      /* 0x010-0x01C 响应寄存器 */
    volatile uint32_t data_port;        /* 0x020 数据端口 */
    volatile uint32_t present_state;    /* 0x024 当前状态 */
    volatile uint8_t  host_ctrl;        /* 0x028 主机控制 */
    volatile uint8_t  pwr_ctrl;         /* 0x029 电源控制 */
    volatile uint8_t  blk_gap_ctrl;     /* 0x02A 块间隙控制 */
    volatile uint8_t  wakeup_ctrl;      /* 0x02B 唤醒控制 */
    volatile uint16_t clock_ctrl;       /* 0x02C 时钟控制 */
    volatile uint8_t  timeout_ctrl;     /* 0x02E 超时控制 */
    volatile uint8_t  sw_reset;         /* 0x02F 软件复位 */
    volatile uint32_t int_status;       /* 0x030 中断状态 */
    volatile uint32_t int_status_en;    /* 0x034 中断状态使能 */
    volatile uint32_t int_signal_en;    /* 0x038 中断信号使能 */
    volatile uint16_t acmd12_err;       /* 0x03C 自动CMD12错误 */
    volatile uint16_t host_ctrl2;       /* 0x03E 主机控制2 */
    volatile uint32_t capabilities[2];  /* 0x040-0x044 能力 */
    volatile uint32_t max_current[2];   /* 0x048-0x04C 最大电流 */
    volatile uint32_t reserved[1];      /* 0x050 保留 */
    volatile uint32_t adma_err_status;  /* 0x054 ADMA错误状态 */
    volatile uint32_t adma_addr_low;    /* 0x058 ADMA地址低32位 */
    volatile uint32_t adma_addr_high;   /* 0x05C ADMA地址高32位 */
} emmc_regs_t;

/* 获取eMMC控制器寄存器指针 */
#define EMMC_REGS   ((emmc_regs_t*)EMMC_BASE_ADDR)

#endif /* __EMMC_REGS_H__ */
