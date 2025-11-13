/**
 * @file emmc_driver.c
 * @brief eMMC驱动实现
 * @description eMMC驱动的主要实现代码
 */

#include "emmc_driver.h"
#include "hal.h"
#include <string.h>

/* ==================== 内部变量 ==================== */
static emmc_device_t g_emmc_device = {0};
static bool g_emmc_initialized = false;
static uint32_t g_clock_freq = 0;

/* ==================== 内部函数声明 ==================== */
static emmc_error_t emmc_reset(void);
static emmc_error_t emmc_power_on(void);
static emmc_error_t emmc_power_off(void);
static emmc_error_t emmc_setup_clock(uint32_t freq_hz);
static emmc_error_t emmc_wait_cmd_complete(uint32_t timeout_ms);
static emmc_error_t emmc_wait_data_complete(uint32_t timeout_ms);
static emmc_error_t emmc_read_response(emmc_response_type_t type, uint32_t *response);
static emmc_error_t emmc_go_idle_state(void);
static emmc_error_t emmc_send_op_cond(uint32_t *ocr);
static emmc_error_t emmc_get_cid(uint32_t *cid);
static emmc_error_t emmc_get_csd(uint32_t *csd);
static emmc_error_t emmc_set_relative_address(uint32_t *rca);
static emmc_error_t emmc_select_card(uint32_t rca);
static emmc_error_t emmc_parse_csd(const uint32_t *csd, emmc_device_t *device);

/* ==================== 公共函数实现 ==================== */

emmc_error_t emmc_init(const emmc_config_t *config)
{
    emmc_error_t ret = EMMC_OK;
    
    if (config == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 检查卡是否存在 */
    if (!emmc_is_present()) {
        return EMMC_ERROR_NOT_PRESENT;
    }
    
    /* 复位控制器 */
    ret = emmc_reset();
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 上电 */
    ret = emmc_power_on();
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 设置时钟 */
    ret = emmc_setup_clock(config->clock_freq);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    g_clock_freq = config->clock_freq;
    
    /* 使能中断 */
    EMMC_WRITE_REG(EMMC_REG_INT_ENABLE, 
                   EMMC_INT_CMD_COMPLETE | EMMC_INT_DATA_COMPLETE | EMMC_INT_ERROR);
    
    /* 发送GO_IDLE_STATE命令 */
    ret = emmc_go_idle_state();
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 发送操作条件命令 */
    uint32_t ocr = 0;
    ret = emmc_send_op_cond(&ocr);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    g_emmc_device.ocr = ocr;
    
    /* 获取CID */
    ret = emmc_get_cid(g_emmc_device.cid);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 设置相对地址 */
    uint32_t rca = 0;
    ret = emmc_set_relative_address(&rca);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    g_emmc_device.rca = rca;
    
    /* 选择卡 */
    ret = emmc_select_card(rca);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 获取CSD */
    ret = emmc_get_csd(g_emmc_device.csd);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 解析CSD获取容量等信息 */
    ret = emmc_parse_csd(g_emmc_device.csd, &g_emmc_device);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 设置总线宽度 */
    if (config->bus_width == 4 || config->bus_width == 8) {
        ret = emmc_set_bus_width(config->bus_width);
        if (ret != EMMC_OK) {
            /* 总线宽度设置失败不影响基本功能 */
        }
    }
    
    /* 设置高速模式 */
    if (config->high_speed) {
        EMMC_SET_BITS(EMMC_REG_CONTROL, EMMC_CTRL_HIGH_SPEED);
    }
    
    g_emmc_device.initialized = true;
    g_emmc_initialized = true;
    
    return EMMC_OK;
}

emmc_error_t emmc_deinit(void)
{
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    /* 禁用中断 */
    EMMC_WRITE_REG(EMMC_REG_INT_ENABLE, 0);
    
    /* 断电 */
    emmc_power_off();
    
    /* 复位控制器 */
    emmc_reset();
    
    memset(&g_emmc_device, 0, sizeof(emmc_device_t));
    g_emmc_initialized = false;
    g_clock_freq = 0;
    
    return EMMC_OK;
}

bool emmc_is_present(void)
{
    uint32_t status = EMMC_READ_REG(EMMC_REG_STATUS);
    return (status & EMMC_STAT_CARD_PRESENT) != 0;
}

emmc_error_t emmc_get_device_info(emmc_device_t *device)
{
    if (device == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    memcpy(device, &g_emmc_device, sizeof(emmc_device_t));
    
    return EMMC_OK;
}

emmc_error_t emmc_read_block(uint32_t block_addr, uint8_t *buffer, uint32_t block_size)
{
    return emmc_read_blocks(block_addr, 1, buffer, block_size);
}

emmc_error_t emmc_write_block(uint32_t block_addr, const uint8_t *buffer, uint32_t block_size)
{
    return emmc_write_blocks(block_addr, 1, buffer, block_size);
}

emmc_error_t emmc_read_blocks(uint32_t block_addr, uint32_t block_count, 
                               uint8_t *buffer, uint32_t block_size)
{
    emmc_error_t ret = EMMC_OK;
    uint8_t cmd;
    uint32_t arg;
    
    if (buffer == NULL || block_size == 0) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    /* 设置块大小 */
    EMMC_WRITE_REG(EMMC_REG_BLOCK_SIZE, block_size);
    EMMC_WRITE_REG(EMMC_REG_BLOCK_COUNT, block_count);
    
    /* 设置数据控制 */
    EMMC_WRITE_REG(EMMC_REG_DATA_CONTROL, 
                   EMMC_DATA_CTRL_ENABLE | (0 << EMMC_DATA_CTRL_DIRECTION));
    
    /* 设置数据长度 */
    EMMC_WRITE_REG(EMMC_REG_DATA_LENGTH, block_count * block_size);
    
    /* 选择命令 */
    if (block_count == 1) {
        cmd = EMMC_CMD_READ_SINGLE_BLOCK;
    } else {
        cmd = EMMC_CMD_READ_MULTIPLE_BLOCK;
    }
    
    /* 计算地址 */
    if (g_emmc_device.high_capacity) {
        arg = block_addr;
    } else {
        arg = block_addr * block_size;
    }
    
    /* 发送读命令 */
    ret = emmc_send_command(cmd, arg, EMMC_RESPONSE_R1, NULL);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 等待数据完成 */
    ret = emmc_wait_data_complete(5000);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 从FIFO读取数据 */
    uint32_t total_bytes = block_count * block_size;
    uint32_t words = (total_bytes + 3) / 4;  /* 向上取整到4字节 */
    
    for (uint32_t i = 0; i < words; i++) {
        uint32_t data = EMMC_READ_REG(EMMC_REG_FIFO);
        uint32_t offset = i * 4;
        
        if (offset < total_bytes) {
            buffer[offset] = (data >> 0) & 0xFF;
            if (offset + 1 < total_bytes) buffer[offset + 1] = (data >> 8) & 0xFF;
            if (offset + 2 < total_bytes) buffer[offset + 2] = (data >> 16) & 0xFF;
            if (offset + 3 < total_bytes) buffer[offset + 3] = (data >> 24) & 0xFF;
        }
    }
    
    /* 如果是多块读取，发送停止命令 */
    if (block_count > 1) {
        ret = emmc_send_command(EMMC_CMD_STOP_TRANSMISSION, 0, EMMC_RESPONSE_R1B, NULL);
    }
    
    return ret;
}

emmc_error_t emmc_write_blocks(uint32_t block_addr, uint32_t block_count, 
                                const uint8_t *buffer, uint32_t block_size)
{
    emmc_error_t ret = EMMC_OK;
    uint8_t cmd;
    uint32_t arg;
    
    if (buffer == NULL || block_size == 0) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    /* 设置块大小 */
    EMMC_WRITE_REG(EMMC_REG_BLOCK_SIZE, block_size);
    EMMC_WRITE_REG(EMMC_REG_BLOCK_COUNT, block_count);
    
    /* 设置数据控制 */
    EMMC_WRITE_REG(EMMC_REG_DATA_CONTROL, 
                   EMMC_DATA_CTRL_ENABLE | (1 << EMMC_DATA_CTRL_DIRECTION));
    
    /* 设置数据长度 */
    EMMC_WRITE_REG(EMMC_REG_DATA_LENGTH, block_count * block_size);
    
    /* 写入数据到FIFO */
    uint32_t total_bytes = block_count * block_size;
    uint32_t words = (total_bytes + 3) / 4;
    
    for (uint32_t i = 0; i < words; i++) {
        uint32_t data = 0;
        uint32_t offset = i * 4;
        
        if (offset < total_bytes) {
            data |= ((uint32_t)buffer[offset]) << 0;
            if (offset + 1 < total_bytes) data |= ((uint32_t)buffer[offset + 1]) << 8;
            if (offset + 2 < total_bytes) data |= ((uint32_t)buffer[offset + 2]) << 16;
            if (offset + 3 < total_bytes) data |= ((uint32_t)buffer[offset + 3]) << 24;
        }
        
        EMMC_WRITE_REG(EMMC_REG_FIFO, data);
    }
    
    /* 选择命令 */
    if (block_count == 1) {
        cmd = EMMC_CMD_WRITE_BLOCK;
    } else {
        cmd = EMMC_CMD_WRITE_MULTIPLE_BLOCK;
    }
    
    /* 计算地址 */
    if (g_emmc_device.high_capacity) {
        arg = block_addr;
    } else {
        arg = block_addr * block_size;
    }
    
    /* 发送写命令 */
    ret = emmc_send_command(cmd, arg, EMMC_RESPONSE_R1, NULL);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 等待数据完成 */
    ret = emmc_wait_data_complete(5000);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 如果是多块写入，发送停止命令 */
    if (block_count > 1) {
        ret = emmc_send_command(EMMC_CMD_STOP_TRANSMISSION, 0, EMMC_RESPONSE_R1B, NULL);
    }
    
    return ret;
}

emmc_error_t emmc_erase_blocks(uint32_t start_block, uint32_t end_block)
{
    emmc_error_t ret = EMMC_OK;
    uint32_t start_addr, end_addr;
    
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    if (g_emmc_device.high_capacity) {
        start_addr = start_block;
        end_addr = end_block;
    } else {
        start_addr = start_block * g_emmc_device.block_size;
        end_addr = end_block * g_emmc_device.block_size;
    }
    
    /* 发送擦除起始地址命令 */
    ret = emmc_send_command(EMMC_CMD_ERASE_GROUP_START, start_addr, EMMC_RESPONSE_R1, NULL);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 发送擦除结束地址命令 */
    ret = emmc_send_command(EMMC_CMD_ERASE_GROUP_END, end_addr, EMMC_RESPONSE_R1, NULL);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 发送擦除命令 */
    ret = emmc_send_command(EMMC_CMD_ERASE, 0, EMMC_RESPONSE_R1B, NULL);
    
    return ret;
}

emmc_error_t emmc_send_command(uint8_t cmd, uint32_t arg, 
                                emmc_response_type_t response_type, 
                                uint32_t *response)
{
    emmc_error_t ret = EMMC_OK;
    uint32_t cmd_reg = 0;
    
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    /* 清除命令完成中断 */
    EMMC_WRITE_REG(EMMC_REG_INT_STATUS, EMMC_INT_CMD_COMPLETE);
    
    /* 设置命令参数 */
    EMMC_WRITE_REG(EMMC_REG_CMD_ARG, arg);
    
    /* 构建命令寄存器 */
    cmd_reg = cmd & EMMC_CMD_INDEX_MASK;
    
    if (response_type != EMMC_RESPONSE_NONE) {
        cmd_reg |= EMMC_CMD_RESPONSE;
        cmd_reg |= EMMC_CMD_CHECK_RESP_CRC;
        
        if (response_type == EMMC_RESPONSE_R2 || response_type == EMMC_RESPONSE_R3) {
            cmd_reg |= EMMC_CMD_LONG_RESPONSE;
        }
        
        if (response_type == EMMC_RESPONSE_R1B) {
            cmd_reg |= EMMC_CMD_WAIT_PRVDATA;
        }
    }
    
    /* 发送命令 */
    EMMC_WRITE_REG(EMMC_REG_CMD, cmd_reg);
    
    /* 等待命令完成 */
    ret = emmc_wait_cmd_complete(5000);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 读取响应 */
    if (response != NULL && response_type != EMMC_RESPONSE_NONE) {
        ret = emmc_read_response(response_type, response);
    }
    
    return ret;
}

emmc_error_t emmc_wait_ready(uint32_t timeout_ms)
{
    uint32_t status;
    
    while (timeout_ms > 0) {
        status = EMMC_READ_REG(EMMC_REG_STATUS);
        
        if (!(status & EMMC_STAT_BUSY)) {
            return EMMC_OK;
        }
        
        /* 延时1ms */
        hal_delay_ms(1);
        timeout_ms--;
    }
    
    return EMMC_ERROR_TIMEOUT;
}

emmc_error_t emmc_set_clock(uint32_t freq_hz)
{
    return emmc_setup_clock(freq_hz);
}

emmc_error_t emmc_set_bus_width(uint8_t width)
{
    emmc_error_t ret = EMMC_OK;
    uint32_t arg = 0;
    
    if (!g_emmc_initialized) {
        return EMMC_ERROR_NOT_INITIALIZED;
    }
    
    /* 发送应用命令 */
    ret = emmc_send_command(EMMC_CMD_APP_CMD, g_emmc_device.rca << 16, EMMC_RESPONSE_R1, NULL);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 设置总线宽度参数 */
    switch (width) {
        case 1:
            arg = 0;
            break;
        case 4:
            arg = 2;
            break;
        case 8:
            arg = 3;
            break;
        default:
            return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 发送设置总线宽度命令 */
    ret = emmc_send_command(EMMC_CMD_APP_SET_BUS_WIDTH, arg, EMMC_RESPONSE_R1, NULL);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    /* 更新控制器设置 */
    EMMC_CLEAR_BITS(EMMC_REG_CONTROL, EMMC_CTRL_4BIT_MODE | EMMC_CTRL_8BIT_MODE);
    
    if (width == 4) {
        EMMC_SET_BITS(EMMC_REG_CONTROL, EMMC_CTRL_4BIT_MODE);
    } else if (width == 8) {
        EMMC_SET_BITS(EMMC_REG_CONTROL, EMMC_CTRL_8BIT_MODE);
    }
    
    return EMMC_OK;
}

uint32_t emmc_get_status(void)
{
    return EMMC_READ_REG(EMMC_REG_STATUS);
}

void emmc_clear_interrupt(uint32_t int_mask)
{
    EMMC_WRITE_REG(EMMC_REG_INT_STATUS, int_mask);
}

void emmc_interrupt_handler(void)
{
    uint32_t int_status = EMMC_READ_REG(EMMC_REG_INT_STATUS);
    
    /* 处理各种中断 */
    if (int_status & EMMC_INT_ERROR) {
        /* 错误处理 */
    }
    
    if (int_status & EMMC_INT_CMD_COMPLETE) {
        /* 命令完成处理 */
    }
    
    if (int_status & EMMC_INT_DATA_COMPLETE) {
        /* 数据完成处理 */
    }
    
    /* 清除中断标志 */
    emmc_clear_interrupt(int_status);
}

/* ==================== 内部函数实现 ==================== */

static emmc_error_t emmc_reset(void)
{
    /* 软件复位 */
    EMMC_SET_BITS(EMMC_REG_CONTROL, EMMC_CTRL_RESET);
    
    /* 等待复位完成 */
    uint32_t timeout = 1000;
    while (timeout > 0) {
        if (!(EMMC_READ_REG(EMMC_REG_CONTROL) & EMMC_CTRL_RESET)) {
            break;
        }
        timeout--;
    }
    
    if (timeout == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    return EMMC_OK;
}

static emmc_error_t emmc_power_on(void)
{
    /* 设置电压选择（3.3V） */
    uint32_t pwr_reg = EMMC_PWR_ON | (0x7 << EMMC_PWR_VOLTAGE_SHIFT);
    EMMC_WRITE_REG(EMMC_REG_POWER_CONTROL, pwr_reg);
    
    /* 延时等待电源稳定 */
    hal_delay_ms(10);
    
    return EMMC_OK;
}

static emmc_error_t emmc_power_off(void)
{
    EMMC_CLEAR_BITS(EMMC_REG_POWER_CONTROL, EMMC_PWR_ON);
    return EMMC_OK;
}

static emmc_error_t emmc_setup_clock(uint32_t freq_hz)
{
    /* 假设系统时钟为100MHz，计算分频器 */
    uint32_t sys_clk = 100000000;  /* 100MHz */
    uint32_t divider = (sys_clk + freq_hz - 1) / freq_hz;  /* 向上取整 */
    
    if (divider < 2) divider = 2;
    if (divider > 256) divider = 256;
    
    /* 设置时钟分频器 */
    uint32_t clk_reg = EMMC_CLK_ENABLE | ((divider << EMMC_CLK_DIVIDER_SHIFT) & EMMC_CLK_DIVIDER_MASK);
    EMMC_WRITE_REG(EMMC_REG_CLOCK_CONTROL, clk_reg);
    
    return EMMC_OK;
}

static emmc_error_t emmc_wait_cmd_complete(uint32_t timeout_ms)
{
    uint32_t status;
    
    while (timeout_ms > 0) {
        status = EMMC_READ_REG(EMMC_REG_STATUS);
        
        if (status & EMMC_STAT_CMD_COMPLETE) {
            /* 检查错误 */
            if (status & EMMC_STAT_ERROR) {
                return EMMC_ERROR_CMD;
            }
            return EMMC_OK;
        }
        
        /* 延时1ms */
        hal_delay_ms(1);
        timeout_ms--;
    }
    
    return EMMC_ERROR_TIMEOUT;
}

static emmc_error_t emmc_wait_data_complete(uint32_t timeout_ms)
{
    uint32_t status;
    
    while (timeout_ms > 0) {
        status = EMMC_READ_REG(EMMC_REG_STATUS);
        
        if (status & EMMC_STAT_DATA_COMPLETE) {
            /* 检查错误 */
            if (status & EMMC_STAT_ERROR) {
                return EMMC_ERROR_DATA;
            }
            return EMMC_OK;
        }
        
        /* 延时1ms */
        hal_delay_ms(1);
        timeout_ms--;
    }
    
    return EMMC_ERROR_TIMEOUT;
}

static emmc_error_t emmc_read_response(emmc_response_type_t type, uint32_t *response)
{
    if (response == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    switch (type) {
        case EMMC_RESPONSE_R1:
        case EMMC_RESPONSE_R1B:
        case EMMC_RESPONSE_R3:
        case EMMC_RESPONSE_R6:
            response[0] = EMMC_READ_REG(EMMC_REG_RESP0);
            break;
            
        case EMMC_RESPONSE_R2:
            response[0] = EMMC_READ_REG(EMMC_REG_RESP0);
            response[1] = EMMC_READ_REG(EMMC_REG_RESP1);
            response[2] = EMMC_READ_REG(EMMC_REG_RESP2);
            response[3] = EMMC_READ_REG(EMMC_REG_RESP3);
            break;
            
        default:
            return EMMC_ERROR_INVALID_PARAM;
    }
    
    return EMMC_OK;
}

static emmc_error_t emmc_go_idle_state(void)
{
    return emmc_send_command(EMMC_CMD_GO_IDLE_STATE, 0, EMMC_RESPONSE_NONE, NULL);
}

static emmc_error_t emmc_send_op_cond(uint32_t *ocr)
{
    emmc_error_t ret;
    uint32_t response[1];
    uint32_t arg = 0x40FF8000;  /* 高容量支持 + 电压范围 */
    
    /* 发送操作条件命令（CMD1） */
    ret = emmc_send_command(EMMC_CMD_SEND_OP_COND, arg, EMMC_RESPONSE_R3, response);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    if (ocr != NULL) {
        *ocr = response[0];
    }
    
    return EMMC_OK;
}

static emmc_error_t emmc_get_cid(uint32_t *cid)
{
    emmc_error_t ret;
    uint32_t response[4];
    
    ret = emmc_send_command(EMMC_CMD_ALL_SEND_CID, 0, EMMC_RESPONSE_R2, response);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    if (cid != NULL) {
        cid[0] = response[0];
        cid[1] = response[1];
        cid[2] = response[2];
        cid[3] = response[3];
    }
    
    return EMMC_OK;
}

static emmc_error_t emmc_get_csd(uint32_t *csd)
{
    emmc_error_t ret;
    uint32_t response[4];
    
    ret = emmc_send_command(EMMC_CMD_SEND_CSD, g_emmc_device.rca << 16, EMMC_RESPONSE_R2, response);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    if (csd != NULL) {
        csd[0] = response[0];
        csd[1] = response[1];
        csd[2] = response[2];
        csd[3] = response[3];
    }
    
    return EMMC_OK;
}

static emmc_error_t emmc_set_relative_address(uint32_t *rca)
{
    emmc_error_t ret;
    uint32_t response[1];
    
    ret = emmc_send_command(EMMC_CMD_SEND_RELATIVE_ADDR, 0, EMMC_RESPONSE_R6, response);
    if (ret != EMMC_OK) {
        return ret;
    }
    
    if (rca != NULL) {
        *rca = (response[0] >> 16) & 0xFFFF;
    }
    
    return EMMC_OK;
}

static emmc_error_t emmc_select_card(uint32_t rca)
{
    return emmc_send_command(EMMC_CMD_SELECT_CARD, rca << 16, EMMC_RESPONSE_R1, NULL);
}

static emmc_error_t emmc_parse_csd(const uint32_t *csd, emmc_device_t *device)
{
    if (csd == NULL || device == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 简化的CSD解析 */
    /* CSD结构复杂，这里只做基本解析 */
    
    /* 检查CSD版本 */
    uint8_t csd_structure = (csd[0] >> 30) & 0x3;
    
    if (csd_structure == 1) {
        /* CSD版本2.0 - 高容量卡 */
        device->high_capacity = true;
        uint32_t c_size = ((csd[1] >> 16) & 0x3F) | ((csd[2] & 0x3) << 6);
        device->capacity = (uint32_t)(c_size + 1) * 512 * 1024;  /* KB转字节 */
    } else {
        /* CSD版本1.0 - 标准容量卡 */
        device->high_capacity = false;
        uint32_t c_size = ((csd[1] >> 16) & 0x3FF) | ((csd[2] & 0x3) << 10);
        uint32_t c_size_mult = ((csd[2] >> 15) & 0x7) + 2;
        uint32_t read_bl_len = (csd[1] >> 16) & 0xF;
        uint32_t block_len = 1 << read_bl_len;
        device->capacity = (c_size + 1) * (1 << c_size_mult) * block_len;
    }
    
    /* 设置块大小（通常为512字节） */
    device->block_size = 512;
    device->block_count = device->capacity / device->block_size;
    
    return EMMC_OK;
}
