/**
 * @file emmc_types.h
 * @brief eMMC固件类型定义
 * @version 1.0
 * @date 2025-11-13
 */

#ifndef __EMMC_TYPES_H__
#define __EMMC_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

/* 返回值定义 */
typedef enum {
    EMMC_SUCCESS = 0,           /* 成功 */
    EMMC_ERROR_TIMEOUT,         /* 超时错误 */
    EMMC_ERROR_CRC,             /* CRC错误 */
    EMMC_ERROR_INVALID_PARAM,   /* 无效参数 */
    EMMC_ERROR_NOT_INIT,        /* 未初始化 */
    EMMC_ERROR_BUSY,            /* 设备忙 */
    EMMC_ERROR_NO_CARD,         /* 无卡 */
    EMMC_ERROR_COMM,            /* 通信错误 */
    EMMC_ERROR_WRITE_PROTECT,   /* 写保护 */
    EMMC_ERROR_UNKNOWN          /* 未知错误 */
} emmc_status_t;

/* eMMC总线宽度 */
typedef enum {
    EMMC_BUS_WIDTH_1BIT = 0,
    EMMC_BUS_WIDTH_4BIT = 1,
    EMMC_BUS_WIDTH_8BIT = 2
} emmc_bus_width_t;

/* eMMC速度模式 */
typedef enum {
    EMMC_SPEED_MODE_LEGACY = 0,     /* 传统模式 (最高26MHz) */
    EMMC_SPEED_MODE_HIGH_SPEED,     /* 高速模式 (最高52MHz) */
    EMMC_SPEED_MODE_HS200,          /* HS200模式 (最高200MHz) */
    EMMC_SPEED_MODE_HS400,          /* HS400模式 (最高400MHz) */
} emmc_speed_mode_t;

/* eMMC电压 */
typedef enum {
    EMMC_VOLTAGE_1V8 = 0,
    EMMC_VOLTAGE_3V3 = 1
} emmc_voltage_t;

/* eMMC卡状态 */
typedef enum {
    EMMC_CARD_STATE_IDLE = 0,
    EMMC_CARD_STATE_READY,
    EMMC_CARD_STATE_IDENT,
    EMMC_CARD_STATE_STBY,
    EMMC_CARD_STATE_TRAN,
    EMMC_CARD_STATE_DATA,
    EMMC_CARD_STATE_RCV,
    EMMC_CARD_STATE_PRG,
    EMMC_CARD_STATE_DIS
} emmc_card_state_t;

/* eMMC卡信息 */
typedef struct {
    uint8_t  cid[16];               /* 卡识别号 */
    uint8_t  csd[16];               /* 卡特定数据 */
    uint8_t  ext_csd[512];          /* 扩展CSD */
    uint32_t rca;                   /* 相对卡地址 */
    uint32_t ocr;                   /* 操作条件寄存器 */
    uint64_t capacity;              /* 容量（字节） */
    uint32_t block_size;            /* 块大小 */
    uint32_t block_count;           /* 块数量 */
    emmc_bus_width_t bus_width;     /* 总线宽度 */
    emmc_speed_mode_t speed_mode;   /* 速度模式 */
    bool     is_initialized;        /* 是否已初始化 */
} emmc_card_info_t;

/* eMMC命令结构 */
typedef struct {
    uint32_t cmd_idx;               /* 命令索引 */
    uint32_t cmd_arg;               /* 命令参数 */
    uint32_t response[4];           /* 响应数据 */
    uint32_t flags;                 /* 命令标志 */
} emmc_cmd_t;

/* eMMC数据传输结构 */
typedef struct {
    uint8_t*  data_ptr;             /* 数据缓冲区指针 */
    uint32_t  block_size;           /* 块大小 */
    uint32_t  block_count;          /* 块数量 */
    uint32_t  flags;                /* 传输标志 */
} emmc_data_t;

/* DMA描述符结构（ADMA2） */
typedef struct {
    uint16_t attr;                  /* 属性 */
    uint16_t len;                   /* 长度 */
    uint32_t addr_low;              /* 地址低32位 */
    uint32_t addr_high;             /* 地址高32位 (64位模式) */
} emmc_adma_desc_t;

/* ADMA描述符属性位 */
#define ADMA_DESC_ATTR_VALID    (1 << 0)
#define ADMA_DESC_ATTR_END      (1 << 1)
#define ADMA_DESC_ATTR_INT      (1 << 2)
#define ADMA_DESC_ATTR_ACT_NOP  (0 << 4)
#define ADMA_DESC_ATTR_ACT_TRAN (2 << 4)
#define ADMA_DESC_ATTR_ACT_LINK (3 << 4)

/* eMMC配置结构 */
typedef struct {
    uint32_t base_clock;            /* 基础时钟频率(Hz) */
    uint32_t timeout_clk;           /* 超时时钟频率(KHz) */
    emmc_bus_width_t bus_width;     /* 总线宽度 */
    emmc_voltage_t voltage;         /* 工作电压 */
    bool dma_enabled;               /* DMA使能 */
    bool high_speed_enabled;        /* 高速模式使能 */
} emmc_config_t;

#endif /* __EMMC_TYPES_H__ */
