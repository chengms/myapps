/**
 * @file hal.h
 * @brief 硬件抽象层接口
 * @description 提供延时、时间等基础功能接口
 */

#ifndef HAL_H
#define HAL_H

#include <stdint.h>

/**
 * @brief 微秒级延时
 * @param us 微秒数
 */
void hal_delay_us(uint32_t us);

/**
 * @brief 毫秒级延时
 * @param ms 毫秒数
 */
void hal_delay_ms(uint32_t ms);

/**
 * @brief 获取系统时间（毫秒）
 * @return 系统时间（毫秒）
 */
uint32_t hal_get_tick_ms(void);

#endif /* HAL_H */
