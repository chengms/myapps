/**
 * @file emmc_driver.h
 * @brief eMMC驱动接口定义
 * @description eMMC驱动的主要API接口
 */

#ifndef EMMC_DRIVER_H
#define EMMC_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "emmc_registers.h"

/* ==================== 类型定义 ==================== */

/**
 * @brief eMMC设备信息结构
 */
typedef struct {
    uint32_t cid[4];              /* 卡识别寄存器 */
    uint32_t csd[4];              /* 卡特定数据寄存器 */
    uint32_t ocr;                 /* 操作条件寄存器 */
    uint32_t rca;                 /* 相对卡地址 */
    uint32_t capacity;            /* 容量（字节） */
    uint32_t block_size;          /* 块大小（字节） */
    uint32_t block_count;         /* 块数量 */
    bool initialized;             /* 初始化标志 */
    bool high_capacity;           /* 高容量卡标志 */
} emmc_device_t;

/**
 * @brief eMMC配置结构
 */
typedef struct {
    uint32_t clock_freq;          /* 时钟频率（Hz） */
    uint32_t bus_width;           /* 总线宽度（1/4/8位） */
    bool dma_enable;              /* DMA使能 */
    bool high_speed;              /* 高速模式 */
} emmc_config_t;

/* ==================== 函数声明 ==================== */

/**
 * @brief 初始化eMMC控制器
 * @param config 配置参数
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_init(const emmc_config_t *config);

/**
 * @brief 反初始化eMMC控制器
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_deinit(void);

/**
 * @brief 检测eMMC卡是否存在
 * @return true 卡存在，false 卡不存在
 */
bool emmc_is_present(void);

/**
 * @brief 获取eMMC设备信息
 * @param device 设备信息结构指针
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_get_device_info(emmc_device_t *device);

/**
 * @brief 读取单个块
 * @param block_addr 块地址
 * @param buffer 数据缓冲区
 * @param block_size 块大小
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_read_block(uint32_t block_addr, uint8_t *buffer, uint32_t block_size);

/**
 * @brief 写入单个块
 * @param block_addr 块地址
 * @param buffer 数据缓冲区
 * @param block_size 块大小
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_write_block(uint32_t block_addr, const uint8_t *buffer, uint32_t block_size);

/**
 * @brief 读取多个块
 * @param block_addr 起始块地址
 * @param block_count 块数量
 * @param buffer 数据缓冲区
 * @param block_size 块大小
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_read_blocks(uint32_t block_addr, uint32_t block_count, 
                               uint8_t *buffer, uint32_t block_size);

/**
 * @brief 写入多个块
 * @param block_addr 起始块地址
 * @param block_count 块数量
 * @param buffer 数据缓冲区
 * @param block_size 块大小
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_write_blocks(uint32_t block_addr, uint32_t block_count, 
                                const uint8_t *buffer, uint32_t block_size);

/**
 * @brief 擦除块
 * @param start_block 起始块地址
 * @param end_block 结束块地址
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_erase_blocks(uint32_t start_block, uint32_t end_block);

/**
 * @brief 发送命令
 * @param cmd 命令索引
 * @param arg 命令参数
 * @param response_type 响应类型
 * @param response 响应缓冲区（可为NULL）
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_send_command(uint8_t cmd, uint32_t arg, 
                                emmc_response_type_t response_type, 
                                uint32_t *response);

/**
 * @brief 等待卡就绪
 * @param timeout_ms 超时时间（毫秒）
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_wait_ready(uint32_t timeout_ms);

/**
 * @brief 设置时钟频率
 * @param freq_hz 频率（Hz）
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_set_clock(uint32_t freq_hz);

/**
 * @brief 设置总线宽度
 * @param width 总线宽度（1/4/8）
 * @return emmc_error_t 错误代码
 */
emmc_error_t emmc_set_bus_width(uint8_t width);

/**
 * @brief 获取状态
 * @return uint32_t 状态寄存器值
 */
uint32_t emmc_get_status(void);

/**
 * @brief 清除中断标志
 * @param int_mask 中断掩码
 */
void emmc_clear_interrupt(uint32_t int_mask);

/**
 * @brief 中断处理函数
 */
void emmc_interrupt_handler(void);

#endif /* EMMC_DRIVER_H */
