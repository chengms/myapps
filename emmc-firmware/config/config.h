/**
 * @file config.h
 * @brief eMMC固件配置文件
 * @description 用户可配置的参数
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ==================== 硬件配置 ==================== */

/* eMMC控制器基地址 */
#define EMMC_BASE_ADDRESS        0x40000000

/* 系统时钟频率（Hz） */
#define SYSTEM_CLOCK_FREQ        100000000  /* 100MHz */

/* ==================== 功能配置 ==================== */

/* 默认时钟频率（Hz） */
#define EMMC_DEFAULT_CLOCK_FREQ  25000000   /* 25MHz */

/* 默认总线宽度（1/4/8） */
#define EMMC_DEFAULT_BUS_WIDTH   4

/* 默认块大小（字节） */
#define EMMC_DEFAULT_BLOCK_SIZE  512

/* DMA使能 */
#define EMMC_DMA_ENABLE          0

/* 高速模式使能 */
#define EMMC_HIGH_SPEED_ENABLE   0

/* ==================== 超时配置 ==================== */

/* 命令超时（毫秒） */
#define EMMC_CMD_TIMEOUT_MS      5000

/* 数据超时（毫秒） */
#define EMMC_DATA_TIMEOUT_MS     5000

/* 卡检测超时（毫秒） */
#define EMMC_CARD_DETECT_TIMEOUT_MS 1000

/* ==================== 调试配置 ==================== */

/* 调试输出使能 */
#define EMMC_DEBUG_ENABLE        1

/* 日志级别 */
#define EMMC_LOG_LEVEL_INFO      1
#define EMMC_LOG_LEVEL_WARNING   2
#define EMMC_LOG_LEVEL_ERROR     3

#define EMMC_LOG_LEVEL           EMMC_LOG_LEVEL_INFO

#endif /* CONFIG_H */
