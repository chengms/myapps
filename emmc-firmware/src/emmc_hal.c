/**
 * @file emmc_hal.c
 * @brief eMMC硬件抽象层实现
 * @version 1.0
 * @date 2025-11-13
 */

#include "emmc_hal.h"
#include <string.h>

/* 全局变量 */
static emmc_config_t g_emmc_config;
static volatile uint32_t g_tick_counter = 0; /* 系统滴答计数器 */

/**
 * @brief 初始化eMMC控制器硬件
 */
emmc_status_t emmc_hal_init(const emmc_config_t* config)
{
    if (config == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 保存配置 */
    memcpy(&g_emmc_config, config, sizeof(emmc_config_t));
    
    /* 复位控制器 */
    EMMC_REG_WRITE(EMMC_SW_RESET, EMMC_RESET_ALL);
    emmc_hal_delay_ms(10);
    
    /* 等待复位完成 */
    uint32_t timeout = 1000;
    while ((EMMC_REG_READ(EMMC_SW_RESET) & EMMC_RESET_ALL) && timeout--) {
        emmc_hal_delay_ms(1);
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    /* 清除所有中断状态 */
    EMMC_REG_WRITE(EMMC_INT_STATUS, 0xFFFFFFFF);
    
    /* 配置中断使能 */
    uint32_t int_mask = EMMC_INT_CMD_DONE | EMMC_INT_XFER_DONE | 
                        EMMC_INT_ERROR | EMMC_INT_CARD_INSERT |
                        EMMC_INT_CARD_REMOVE;
    EMMC_REG_WRITE(EMMC_INT_STATUS_EN, int_mask);
    EMMC_REG_WRITE(EMMC_INT_SIGNAL_EN, 0); /* 初始禁用所有中断信号 */
    
    /* 设置超时时间 */
    EMMC_REG_WRITE(EMMC_TIMEOUT_CTRL, 0x0E); /* 最大超时值 */
    
    return EMMC_SUCCESS;
}

/**
 * @brief 反初始化eMMC控制器
 */
emmc_status_t emmc_hal_deinit(void)
{
    /* 禁用所有中断 */
    EMMC_REG_WRITE(EMMC_INT_STATUS_EN, 0);
    EMMC_REG_WRITE(EMMC_INT_SIGNAL_EN, 0);
    
    /* 关闭时钟 */
    EMMC_REG_WRITE(EMMC_CLOCK_CTRL, 0);
    
    /* 关闭电源 */
    EMMC_REG_WRITE(EMMC_PWR_CTRL, 0);
    
    /* 复位控制器 */
    EMMC_REG_WRITE(EMMC_SW_RESET, EMMC_RESET_ALL);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 设置时钟频率
 */
emmc_status_t emmc_hal_set_clock(uint32_t freq_hz)
{
    if (freq_hz == 0) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 禁用SD时钟 */
    EMMC_REG_CLR_BITS(EMMC_CLOCK_CTRL, EMMC_CLK_CARD_EN);
    
    /* 计算分频器 */
    uint32_t base_clock = g_emmc_config.base_clock;
    uint32_t div = 1;
    while ((base_clock / div) > freq_hz && div < 256) {
        div *= 2;
    }
    div /= 2; /* SDMMC分频器实际值 */
    
    /* 设置分频器并使能内部时钟 */
    uint16_t clk_ctrl = (div << EMMC_CLK_DIV_SHIFT) | EMMC_CLK_INT_CLK_EN;
    EMMC_REG_WRITE(EMMC_CLOCK_CTRL, clk_ctrl);
    
    /* 等待内部时钟稳定 */
    uint32_t timeout = 1000;
    while (!(EMMC_REG_READ(EMMC_CLOCK_CTRL) & EMMC_CLK_INT_CLK_STABLE) && timeout--) {
        emmc_hal_delay_us(10);
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    /* 使能SD时钟 */
    EMMC_REG_SET_BITS(EMMC_CLOCK_CTRL, EMMC_CLK_CARD_EN);
    emmc_hal_delay_ms(2);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 设置总线宽度
 */
emmc_status_t emmc_hal_set_bus_width(emmc_bus_width_t width)
{
    uint8_t host_ctrl = EMMC_REG_READ(EMMC_HOST_CTRL);
    
    host_ctrl &= ~EMMC_CTRL_DATA_WIDTH; /* 清除宽度位 */
    
    switch (width) {
        case EMMC_BUS_WIDTH_1BIT:
            /* 1位宽度，位已清除 */
            break;
        case EMMC_BUS_WIDTH_4BIT:
            host_ctrl |= EMMC_CTRL_DATA_WIDTH;
            break;
        case EMMC_BUS_WIDTH_8BIT:
            /* 8位宽度需要在HOST_CTRL2中设置 */
            /* 这里简化处理 */
            break;
        default:
            return EMMC_ERROR_INVALID_PARAM;
    }
    
    EMMC_REG_WRITE(EMMC_HOST_CTRL, host_ctrl);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 设置电源状态
 */
emmc_status_t emmc_hal_set_power(bool on, emmc_voltage_t voltage)
{
    if (!on) {
        EMMC_REG_WRITE(EMMC_PWR_CTRL, 0);
        return EMMC_SUCCESS;
    }
    
    uint8_t pwr_ctrl = 0;
    switch (voltage) {
        case EMMC_VOLTAGE_1V8:
            pwr_ctrl = EMMC_PWR_VOLTAGE_1V8;
            break;
        case EMMC_VOLTAGE_3V3:
            pwr_ctrl = EMMC_PWR_VOLTAGE_3V3;
            break;
        default:
            return EMMC_ERROR_INVALID_PARAM;
    }
    
    pwr_ctrl |= EMMC_PWR_CTRL_ON;
    EMMC_REG_WRITE(EMMC_PWR_CTRL, pwr_ctrl);
    
    /* 等待电源稳定 */
    emmc_hal_delay_ms(5);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 发送命令
 */
emmc_status_t emmc_hal_send_command(emmc_cmd_t* cmd)
{
    if (cmd == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 等待命令线空闲 */
    uint32_t timeout = 1000;
    while ((EMMC_REG_READ(EMMC_PRESENT_STATE) & EMMC_STATE_CMD_INHIBIT) && timeout--) {
        emmc_hal_delay_us(10);
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    /* 如果命令需要数据，等待数据线空闲 */
    if (cmd->flags & EMMC_CMD_DATA_EN) {
        timeout = 1000;
        while ((EMMC_REG_READ(EMMC_PRESENT_STATE) & EMMC_STATE_DAT_INHIBIT) && timeout--) {
            emmc_hal_delay_us(10);
        }
        
        if (timeout == 0) {
            return EMMC_ERROR_TIMEOUT;
        }
    }
    
    /* 清除命令相关中断 */
    EMMC_REG_WRITE(EMMC_INT_STATUS, EMMC_INT_CMD_DONE | EMMC_INT_ERROR);
    
    /* 设置命令参数 */
    EMMC_REG_WRITE(EMMC_ARGUMENT, cmd->cmd_arg);
    
    /* 设置并发送命令 */
    uint16_t cmd_reg = (cmd->cmd_idx << 8) | cmd->flags;
    EMMC_REG_WRITE(EMMC_COMMAND, cmd_reg);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 等待命令完成
 */
emmc_status_t emmc_hal_wait_command_done(uint32_t timeout_ms)
{
    uint32_t start_tick = emmc_hal_get_tick_ms();
    
    while (1) {
        uint32_t int_status = EMMC_REG_READ(EMMC_INT_STATUS);
        
        /* 检查错误 */
        if (int_status & EMMC_INT_ERROR) {
            EMMC_REG_WRITE(EMMC_INT_STATUS, int_status);
            if (int_status & EMMC_INT_TIMEOUT) {
                return EMMC_ERROR_TIMEOUT;
            } else if (int_status & EMMC_INT_CRC_ERROR) {
                return EMMC_ERROR_CRC;
            } else {
                return EMMC_ERROR_COMM;
            }
        }
        
        /* 检查完成 */
        if (int_status & EMMC_INT_CMD_DONE) {
            EMMC_REG_WRITE(EMMC_INT_STATUS, EMMC_INT_CMD_DONE);
            return EMMC_SUCCESS;
        }
        
        /* 检查超时 */
        if ((emmc_hal_get_tick_ms() - start_tick) > timeout_ms) {
            return EMMC_ERROR_TIMEOUT;
        }
        
        emmc_hal_delay_us(10);
    }
}

/**
 * @brief 读取数据
 */
emmc_status_t emmc_hal_read_data(emmc_data_t* data)
{
    if (data == NULL || data->data_ptr == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    uint32_t* buf = (uint32_t*)data->data_ptr;
    uint32_t total_words = (data->block_size * data->block_count) / 4;
    uint32_t words_read = 0;
    
    while (words_read < total_words) {
        uint32_t int_status = EMMC_REG_READ(EMMC_INT_STATUS);
        
        /* 检查错误 */
        if (int_status & EMMC_INT_ERROR) {
            return EMMC_ERROR_COMM;
        }
        
        /* 检查缓冲区就绪 */
        if (int_status & EMMC_INT_BUF_RD_READY) {
            /* 读取一个块的数据 */
            uint32_t words_per_block = data->block_size / 4;
            for (uint32_t i = 0; i < words_per_block && words_read < total_words; i++) {
                buf[words_read++] = EMMC_REG_READ(EMMC_DATA_PORT);
            }
            
            /* 清除缓冲区就绪标志 */
            EMMC_REG_WRITE(EMMC_INT_STATUS, EMMC_INT_BUF_RD_READY);
        }
    }
    
    /* 等待传输完成 */
    uint32_t timeout = 5000;
    while (!(EMMC_REG_READ(EMMC_INT_STATUS) & EMMC_INT_XFER_DONE) && timeout--) {
        emmc_hal_delay_ms(1);
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    /* 清除传输完成标志 */
    EMMC_REG_WRITE(EMMC_INT_STATUS, EMMC_INT_XFER_DONE);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 写入数据
 */
emmc_status_t emmc_hal_write_data(const emmc_data_t* data)
{
    if (data == NULL || data->data_ptr == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    uint32_t* buf = (uint32_t*)data->data_ptr;
    uint32_t total_words = (data->block_size * data->block_count) / 4;
    uint32_t words_written = 0;
    
    while (words_written < total_words) {
        uint32_t int_status = EMMC_REG_READ(EMMC_INT_STATUS);
        
        /* 检查错误 */
        if (int_status & EMMC_INT_ERROR) {
            return EMMC_ERROR_COMM;
        }
        
        /* 检查缓冲区就绪 */
        if (int_status & EMMC_INT_BUF_WR_READY) {
            /* 写入一个块的数据 */
            uint32_t words_per_block = data->block_size / 4;
            for (uint32_t i = 0; i < words_per_block && words_written < total_words; i++) {
                EMMC_REG_WRITE(EMMC_DATA_PORT, buf[words_written++]);
            }
            
            /* 清除缓冲区就绪标志 */
            EMMC_REG_WRITE(EMMC_INT_STATUS, EMMC_INT_BUF_WR_READY);
        }
    }
    
    /* 等待传输完成 */
    uint32_t timeout = 5000;
    while (!(EMMC_REG_READ(EMMC_INT_STATUS) & EMMC_INT_XFER_DONE) && timeout--) {
        emmc_hal_delay_ms(1);
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    /* 清除传输完成标志 */
    EMMC_REG_WRITE(EMMC_INT_STATUS, EMMC_INT_XFER_DONE);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 软件复位
 */
emmc_status_t emmc_hal_reset(uint8_t type)
{
    EMMC_REG_WRITE(EMMC_SW_RESET, type);
    
    uint32_t timeout = 1000;
    while ((EMMC_REG_READ(EMMC_SW_RESET) & type) && timeout--) {
        emmc_hal_delay_ms(1);
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    return EMMC_SUCCESS;
}

/**
 * @brief 获取当前状态
 */
uint32_t emmc_hal_get_state(void)
{
    return EMMC_REG_READ(EMMC_PRESENT_STATE);
}

/**
 * @brief 检查是否有卡插入
 */
bool emmc_hal_is_card_present(void)
{
    uint32_t state = EMMC_REG_READ(EMMC_PRESENT_STATE);
    return (state & EMMC_STATE_CARD_INSERT) ? true : false;
}

/**
 * @brief 获取中断状态
 */
uint32_t emmc_hal_get_int_status(void)
{
    return EMMC_REG_READ(EMMC_INT_STATUS);
}

/**
 * @brief 清除中断状态
 */
void emmc_hal_clear_int_status(uint32_t int_mask)
{
    EMMC_REG_WRITE(EMMC_INT_STATUS, int_mask);
}

/**
 * @brief 使能中断
 */
void emmc_hal_enable_int(uint32_t int_mask)
{
    EMMC_REG_SET_BITS(EMMC_INT_SIGNAL_EN, int_mask);
}

/**
 * @brief 禁用中断
 */
void emmc_hal_disable_int(uint32_t int_mask)
{
    EMMC_REG_CLR_BITS(EMMC_INT_SIGNAL_EN, int_mask);
}

/**
 * @brief 配置DMA传输
 */
emmc_status_t emmc_hal_setup_dma(uint32_t addr, uint32_t size, bool is_read)
{
    if (!g_emmc_config.dma_enabled) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 设置DMA地址 */
    EMMC_REG_WRITE(EMMC_SYSADDR, addr);
    
    /* 使能DMA模式 */
    uint8_t host_ctrl = EMMC_REG_READ(EMMC_HOST_CTRL);
    host_ctrl &= ~EMMC_CTRL_DMA_MASK;
    host_ctrl |= EMMC_CTRL_DMA_SDMA; /* 使用SDMA */
    EMMC_REG_WRITE(EMMC_HOST_CTRL, host_ctrl);
    
    return EMMC_SUCCESS;
}

/**
 * @brief 延时函数(毫秒) - 模拟实现
 */
void emmc_hal_delay_ms(uint32_t ms)
{
    /* 这里应该调用实际的系统延时函数 */
    /* 模拟延时 */
    volatile uint32_t count = ms * 10000;
    while (count--);
}

/**
 * @brief 延时函数(微秒) - 模拟实现
 */
void emmc_hal_delay_us(uint32_t us)
{
    /* 这里应该调用实际的系统延时函数 */
    /* 模拟延时 */
    volatile uint32_t count = us * 10;
    while (count--);
}

/**
 * @brief 获取系统时间戳(毫秒) - 模拟实现
 */
uint32_t emmc_hal_get_tick_ms(void)
{
    /* 这里应该返回实际的系统时间戳 */
    /* 模拟实现 */
    return g_tick_counter++;
}
