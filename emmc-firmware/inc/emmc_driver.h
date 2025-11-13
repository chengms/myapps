/**
 * @file emmc_driver.h
 * @brief eMMC驱动程序接口定义
 * @version 1.0
 * @date 2025-11-13
 * 
 * 提供上层应用使用的eMMC驱动接口
 */

#ifndef __EMMC_DRIVER_H__
#define __EMMC_DRIVER_H__

#include "emmc_types.h"

/**
 * @brief 初始化eMMC驱动
 * @param config 配置参数
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_init(const emmc_config_t* config);

/**
 * @brief 反初始化eMMC驱动
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_deinit(void);

/**
 * @brief 获取卡信息
 * @param card_info 输出卡信息
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_get_card_info(emmc_card_info_t* card_info);

/**
 * @brief 读取数据块
 * @param block_addr 块地址
 * @param buffer 数据缓冲区
 * @param block_count 块数量
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_read_blocks(uint32_t block_addr, 
                                      uint8_t* buffer, 
                                      uint32_t block_count);

/**
 * @brief 写入数据块
 * @param block_addr 块地址
 * @param buffer 数据缓冲区
 * @param block_count 块数量
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_write_blocks(uint32_t block_addr, 
                                       const uint8_t* buffer, 
                                       uint32_t block_count);

/**
 * @brief 擦除数据块
 * @param start_addr 起始块地址
 * @param end_addr 结束块地址
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_erase_blocks(uint32_t start_addr, uint32_t end_addr);

/**
 * @brief 获取eMMC容量
 * @return uint64_t 容量（字节）
 */
uint64_t emmc_driver_get_capacity(void);

/**
 * @brief 检查eMMC是否就绪
 * @return bool true=就绪, false=未就绪
 */
bool emmc_driver_is_ready(void);

/**
 * @brief 设置速度模式
 * @param speed_mode 速度模式
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_set_speed_mode(emmc_speed_mode_t speed_mode);

/**
 * @brief 设置总线宽度
 * @param bus_width 总线宽度
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_driver_set_bus_width(emmc_bus_width_t bus_width);

/**
 * @brief 获取卡状态
 * @return emmc_card_state_t 卡状态
 */
emmc_card_state_t emmc_driver_get_card_state(void);

#endif /* __EMMC_DRIVER_H__ */
