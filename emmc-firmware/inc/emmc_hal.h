/**
 * @file emmc_hal.h
 * @brief eMMC硬件抽象层接口定义
 * @version 1.0
 * @date 2025-11-13
 * 
 * 提供eMMC硬件相关的抽象接口
 */

#ifndef __EMMC_HAL_H__
#define __EMMC_HAL_H__

#include "emmc_types.h"
#include "emmc_regs.h"

/**
 * @brief 初始化eMMC控制器硬件
 * @param config 配置参数
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_init(const emmc_config_t* config);

/**
 * @brief 反初始化eMMC控制器
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_deinit(void);

/**
 * @brief 设置时钟频率
 * @param freq_hz 目标频率(Hz)
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_set_clock(uint32_t freq_hz);

/**
 * @brief 设置总线宽度
 * @param width 总线宽度
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_set_bus_width(emmc_bus_width_t width);

/**
 * @brief 设置电源状态
 * @param on 开启或关闭
 * @param voltage 电压选择
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_set_power(bool on, emmc_voltage_t voltage);

/**
 * @brief 发送命令
 * @param cmd 命令结构
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_send_command(emmc_cmd_t* cmd);

/**
 * @brief 等待命令完成
 * @param timeout_ms 超时时间(毫秒)
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_wait_command_done(uint32_t timeout_ms);

/**
 * @brief 读取数据
 * @param data 数据传输结构
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_read_data(emmc_data_t* data);

/**
 * @brief 写入数据
 * @param data 数据传输结构
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_write_data(const emmc_data_t* data);

/**
 * @brief 软件复位
 * @param type 复位类型 (全部/命令/数据)
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_reset(uint8_t type);

/**
 * @brief 获取当前状态
 * @return uint32_t 状态寄存器值
 */
uint32_t emmc_hal_get_state(void);

/**
 * @brief 检查是否有卡插入
 * @return bool true=有卡, false=无卡
 */
bool emmc_hal_is_card_present(void);

/**
 * @brief 获取中断状态
 * @return uint32_t 中断状态
 */
uint32_t emmc_hal_get_int_status(void);

/**
 * @brief 清除中断状态
 * @param int_mask 要清除的中断位
 */
void emmc_hal_clear_int_status(uint32_t int_mask);

/**
 * @brief 使能中断
 * @param int_mask 中断掩码
 */
void emmc_hal_enable_int(uint32_t int_mask);

/**
 * @brief 禁用中断
 * @param int_mask 中断掩码
 */
void emmc_hal_disable_int(uint32_t int_mask);

/**
 * @brief 配置DMA传输
 * @param addr 数据缓冲区地址
 * @param size 数据大小
 * @param is_read 是否为读操作
 * @return emmc_status_t 操作结果
 */
emmc_status_t emmc_hal_setup_dma(uint32_t addr, uint32_t size, bool is_read);

/**
 * @brief 延时函数(毫秒)
 * @param ms 延时时间
 */
void emmc_hal_delay_ms(uint32_t ms);

/**
 * @brief 延时函数(微秒)
 * @param us 延时时间
 */
void emmc_hal_delay_us(uint32_t us);

/**
 * @brief 获取系统时间戳(毫秒)
 * @return uint32_t 时间戳
 */
uint32_t emmc_hal_get_tick_ms(void);

#endif /* __EMMC_HAL_H__ */
