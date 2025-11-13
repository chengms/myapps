/**
 * @file hal.c
 * @brief 硬件抽象层实现
 * @description 提供延时、时间等基础功能的模拟实现
 */

#include <stdint.h>

/* ==================== 延时函数 ==================== */

/**
 * @brief 微秒级延时（模拟实现）
 * @param us 微秒数
 */
void hal_delay_us(uint32_t us)
{
    /* 模拟实现：根据CPU频率计算循环次数 */
    /* 假设CPU频率为100MHz，每个循环约10ns */
    volatile uint32_t count = us * 100;  /* 简化计算 */
    
    while (count--) {
        __asm__ __volatile__("nop");
    }
}

/**
 * @brief 毫秒级延时
 * @param ms 毫秒数
 */
void hal_delay_ms(uint32_t ms)
{
    hal_delay_us(ms * 1000);
}

/* ==================== 时间函数 ==================== */

/**
 * @brief 获取系统时间（毫秒）
 * @return 系统时间（毫秒）
 */
uint32_t hal_get_tick_ms(void)
{
    /* 模拟实现：返回一个递增的计数器值 */
    static uint32_t tick = 0;
    /* 实际实现应该从系统定时器读取 */
    return tick++;
}
