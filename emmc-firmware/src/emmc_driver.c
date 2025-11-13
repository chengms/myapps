/**
 * @file emmc_driver.c
 * @brief eMMC驱动程序实现
 * @version 1.0
 * @date 2025-11-13
 */

#include "emmc_driver.h"
#include "emmc_hal.h"
#include <string.h>

/* 全局卡信息 */
static emmc_card_info_t g_card_info = {0};
static bool g_driver_initialized = false;

/* 内部函数声明 */
static emmc_status_t emmc_send_cmd(uint32_t cmd_idx, uint32_t arg, uint32_t flags, uint32_t* response);
static emmc_status_t emmc_card_identification(void);
static emmc_status_t emmc_card_init_sequence(void);
static emmc_status_t emmc_select_card(void);
static emmc_status_t emmc_set_block_length(uint32_t block_len);
static emmc_card_state_t emmc_parse_card_state(uint32_t status);

/**
 * @brief 初始化eMMC驱动
 */
emmc_status_t emmc_driver_init(const emmc_config_t* config)
{
    emmc_status_t status;
    
    if (config == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 初始化HAL层 */
    status = emmc_hal_init(config);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 上电 */
    status = emmc_hal_set_power(true, config->voltage);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 设置初始时钟（400KHz用于识别模式） */
    status = emmc_hal_set_clock(400000);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 检查卡是否存在 */
    if (!emmc_hal_is_card_present()) {
        return EMMC_ERROR_NO_CARD;
    }
    
    /* 卡识别和初始化序列 */
    status = emmc_card_identification();
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    status = emmc_card_init_sequence();
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 选择卡 */
    status = emmc_select_card();
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 设置高速时钟 */
    if (config->high_speed_enabled) {
        status = emmc_hal_set_clock(52000000); /* 52MHz */
        if (status != EMMC_SUCCESS) {
            return status;
        }
        g_card_info.speed_mode = EMMC_SPEED_MODE_HIGH_SPEED;
    }
    
    /* 设置总线宽度 */
    status = emmc_hal_set_bus_width(config->bus_width);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    g_card_info.bus_width = config->bus_width;
    
    /* 设置块长度 */
    g_card_info.block_size = 512;
    status = emmc_set_block_length(g_card_info.block_size);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    g_card_info.is_initialized = true;
    g_driver_initialized = true;
    
    return EMMC_SUCCESS;
}

/**
 * @brief 反初始化eMMC驱动
 */
emmc_status_t emmc_driver_deinit(void)
{
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    emmc_hal_deinit();
    
    memset(&g_card_info, 0, sizeof(emmc_card_info_t));
    g_driver_initialized = false;
    
    return EMMC_SUCCESS;
}

/**
 * @brief 获取卡信息
 */
emmc_status_t emmc_driver_get_card_info(emmc_card_info_t* card_info)
{
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    if (card_info == NULL) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    memcpy(card_info, &g_card_info, sizeof(emmc_card_info_t));
    
    return EMMC_SUCCESS;
}

/**
 * @brief 读取数据块
 */
emmc_status_t emmc_driver_read_blocks(uint32_t block_addr, 
                                      uint8_t* buffer, 
                                      uint32_t block_count)
{
    emmc_status_t status;
    
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    if (buffer == NULL || block_count == 0) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 检查地址范围 */
    if (block_addr + block_count > g_card_info.block_count) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 设置块大小和块计数 */
    EMMC_REG_WRITE(EMMC_BLKSIZE, g_card_info.block_size);
    EMMC_REG_WRITE(EMMC_BLKCNT, block_count);
    
    /* 设置传输模式 */
    uint16_t xfer_mode = EMMC_XFER_DATA_DIR | EMMC_XFER_BLK_CNT_EN;
    if (block_count > 1) {
        xfer_mode |= EMMC_XFER_MULTI_BLK | EMMC_XFER_ACMD12_EN;
    }
    EMMC_REG_WRITE(EMMC_XFER_MODE, xfer_mode);
    
    /* 发送读命令 */
    uint32_t cmd_idx = (block_count == 1) ? EMMC_CMD17_READ_SINGLE : EMMC_CMD18_READ_MULTIPLE;
    uint32_t cmd_flags = EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | 
                         EMMC_CMD_IDX_EN | EMMC_CMD_DATA_EN;
    
    status = emmc_send_cmd(cmd_idx, block_addr, cmd_flags, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 读取数据 */
    emmc_data_t data = {
        .data_ptr = buffer,
        .block_size = g_card_info.block_size,
        .block_count = block_count,
        .flags = 0
    };
    
    status = emmc_hal_read_data(&data);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    return EMMC_SUCCESS;
}

/**
 * @brief 写入数据块
 */
emmc_status_t emmc_driver_write_blocks(uint32_t block_addr, 
                                       const uint8_t* buffer, 
                                       uint32_t block_count)
{
    emmc_status_t status;
    
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    if (buffer == NULL || block_count == 0) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 检查地址范围 */
    if (block_addr + block_count > g_card_info.block_count) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 设置块大小和块计数 */
    EMMC_REG_WRITE(EMMC_BLKSIZE, g_card_info.block_size);
    EMMC_REG_WRITE(EMMC_BLKCNT, block_count);
    
    /* 设置传输模式 */
    uint16_t xfer_mode = EMMC_XFER_BLK_CNT_EN; /* 写方向，DIR=0 */
    if (block_count > 1) {
        xfer_mode |= EMMC_XFER_MULTI_BLK | EMMC_XFER_ACMD12_EN;
    }
    EMMC_REG_WRITE(EMMC_XFER_MODE, xfer_mode);
    
    /* 发送写命令 */
    uint32_t cmd_idx = (block_count == 1) ? EMMC_CMD24_WRITE_SINGLE : EMMC_CMD25_WRITE_MULTIPLE;
    uint32_t cmd_flags = EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | 
                         EMMC_CMD_IDX_EN | EMMC_CMD_DATA_EN;
    
    status = emmc_send_cmd(cmd_idx, block_addr, cmd_flags, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 写入数据 */
    emmc_data_t data = {
        .data_ptr = (uint8_t*)buffer,
        .block_size = g_card_info.block_size,
        .block_count = block_count,
        .flags = 0
    };
    
    status = emmc_hal_write_data(&data);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    return EMMC_SUCCESS;
}

/**
 * @brief 擦除数据块
 */
emmc_status_t emmc_driver_erase_blocks(uint32_t start_addr, uint32_t end_addr)
{
    emmc_status_t status;
    
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    if (start_addr > end_addr || end_addr >= g_card_info.block_count) {
        return EMMC_ERROR_INVALID_PARAM;
    }
    
    /* 设置擦除起始地址 */
    status = emmc_send_cmd(EMMC_CMD35_ERASE_GROUP_START, start_addr, 
                          EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 设置擦除结束地址 */
    status = emmc_send_cmd(EMMC_CMD36_ERASE_GROUP_END, end_addr, 
                          EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 执行擦除 */
    status = emmc_send_cmd(EMMC_CMD38_ERASE, 0, 
                          EMMC_CMD_RESP_48_BUSY | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    return EMMC_SUCCESS;
}

/**
 * @brief 获取eMMC容量
 */
uint64_t emmc_driver_get_capacity(void)
{
    if (!g_driver_initialized) {
        return 0;
    }
    
    return g_card_info.capacity;
}

/**
 * @brief 检查eMMC是否就绪
 */
bool emmc_driver_is_ready(void)
{
    if (!g_driver_initialized) {
        return false;
    }
    
    return g_card_info.is_initialized;
}

/**
 * @brief 设置速度模式
 */
emmc_status_t emmc_driver_set_speed_mode(emmc_speed_mode_t speed_mode)
{
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    /* 这里应该发送SWITCH命令(CMD6)来改变速度模式 */
    /* 简化实现 */
    g_card_info.speed_mode = speed_mode;
    
    return EMMC_SUCCESS;
}

/**
 * @brief 设置总线宽度
 */
emmc_status_t emmc_driver_set_bus_width(emmc_bus_width_t bus_width)
{
    emmc_status_t status;
    
    if (!g_driver_initialized) {
        return EMMC_ERROR_NOT_INIT;
    }
    
    /* 这里应该发送SWITCH命令(CMD6)来改变总线宽度 */
    /* 然后配置控制器 */
    status = emmc_hal_set_bus_width(bus_width);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    g_card_info.bus_width = bus_width;
    
    return EMMC_SUCCESS;
}

/**
 * @brief 获取卡状态
 */
emmc_card_state_t emmc_driver_get_card_state(void)
{
    if (!g_driver_initialized) {
        return EMMC_CARD_STATE_IDLE;
    }
    
    uint32_t status = 0;
    emmc_status_t ret = emmc_send_cmd(EMMC_CMD13_SEND_STATUS, g_card_info.rca << 16,
                                      EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN,
                                      &status);
    
    if (ret != EMMC_SUCCESS) {
        return EMMC_CARD_STATE_IDLE;
    }
    
    return emmc_parse_card_state(status);
}

/**
 * @brief 发送命令（内部函数）
 */
static emmc_status_t emmc_send_cmd(uint32_t cmd_idx, uint32_t arg, uint32_t flags, uint32_t* response)
{
    emmc_cmd_t cmd = {
        .cmd_idx = cmd_idx,
        .cmd_arg = arg,
        .flags = flags
    };
    
    emmc_status_t status = emmc_hal_send_command(&cmd);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    status = emmc_hal_wait_command_done(1000);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 读取响应 */
    if (response != NULL && (flags & 0x3) != EMMC_CMD_RESP_NONE) {
        *response = EMMC_REG_READ(EMMC_RESPONSE0);
    }
    
    return EMMC_SUCCESS;
}

/**
 * @brief 卡识别流程
 */
static emmc_status_t emmc_card_identification(void)
{
    emmc_status_t status;
    
    /* CMD0: GO_IDLE_STATE */
    status = emmc_send_cmd(EMMC_CMD0_GO_IDLE, 0, EMMC_CMD_RESP_NONE, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    emmc_hal_delay_ms(10);
    
    /* CMD1: SEND_OP_COND - 发送操作条件 */
    uint32_t ocr = 0;
    uint32_t retry = 100;
    while (retry--) {
        status = emmc_send_cmd(EMMC_CMD1_SEND_OP_COND, 0x40FF8080, 
                              EMMC_CMD_RESP_48, &ocr);
        if (status != EMMC_SUCCESS) {
            return status;
        }
        
        /* 检查是否完成初始化（bit 31） */
        if (ocr & 0x80000000) {
            break;
        }
        
        emmc_hal_delay_ms(10);
    }
    
    if (retry == 0) {
        return EMMC_ERROR_TIMEOUT;
    }
    
    g_card_info.ocr = ocr;
    
    return EMMC_SUCCESS;
}

/**
 * @brief 卡初始化序列
 */
static emmc_status_t emmc_card_init_sequence(void)
{
    emmc_status_t status;
    uint32_t cid[4] = {0};
    
    /* CMD2: ALL_SEND_CID - 获取CID */
    status = emmc_send_cmd(EMMC_CMD2_ALL_SEND_CID, 0, EMMC_CMD_RESP_136, &cid[0]);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* 读取完整的CID响应 */
    cid[0] = EMMC_REG_READ(EMMC_RESPONSE0);
    cid[1] = EMMC_REG_READ(EMMC_RESPONSE1);
    cid[2] = EMMC_REG_READ(EMMC_RESPONSE2);
    cid[3] = EMMC_REG_READ(EMMC_RESPONSE3);
    memcpy(g_card_info.cid, cid, 16);
    
    /* CMD3: SET_RELATIVE_ADDR - 设置RCA */
    g_card_info.rca = 0x0001; /* eMMC使用固定的RCA */
    status = emmc_send_cmd(EMMC_CMD3_SET_RELATIVE_ADDR, g_card_info.rca << 16,
                          EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN, NULL);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    /* CMD9: SEND_CSD - 获取CSD */
    uint32_t csd[4] = {0};
    status = emmc_send_cmd(EMMC_CMD9_SEND_CSD, g_card_info.rca << 16, 
                          EMMC_CMD_RESP_136, &csd[0]);
    if (status != EMMC_SUCCESS) {
        return status;
    }
    
    csd[0] = EMMC_REG_READ(EMMC_RESPONSE0);
    csd[1] = EMMC_REG_READ(EMMC_RESPONSE1);
    csd[2] = EMMC_REG_READ(EMMC_RESPONSE2);
    csd[3] = EMMC_REG_READ(EMMC_RESPONSE3);
    memcpy(g_card_info.csd, csd, 16);
    
    /* 从CSD计算容量（简化版本） */
    g_card_info.block_size = 512;
    g_card_info.block_count = 0x1000000; /* 模拟8GB */
    g_card_info.capacity = (uint64_t)g_card_info.block_count * g_card_info.block_size;
    
    return EMMC_SUCCESS;
}

/**
 * @brief 选择卡
 */
static emmc_status_t emmc_select_card(void)
{
    /* CMD7: SELECT_CARD */
    return emmc_send_cmd(EMMC_CMD7_SELECT_CARD, g_card_info.rca << 16,
                        EMMC_CMD_RESP_48_BUSY | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN, NULL);
}

/**
 * @brief 设置块长度
 */
static emmc_status_t emmc_set_block_length(uint32_t block_len)
{
    /* CMD16: SET_BLOCKLEN */
    return emmc_send_cmd(EMMC_CMD16_SET_BLOCKLEN, block_len,
                        EMMC_CMD_RESP_48 | EMMC_CMD_CRC_EN | EMMC_CMD_IDX_EN, NULL);
}

/**
 * @brief 解析卡状态
 */
static emmc_card_state_t emmc_parse_card_state(uint32_t status)
{
    /* 卡状态位于bits [12:9] */
    uint32_t state = (status >> 9) & 0x0F;
    
    switch (state) {
        case 0: return EMMC_CARD_STATE_IDLE;
        case 1: return EMMC_CARD_STATE_READY;
        case 2: return EMMC_CARD_STATE_IDENT;
        case 3: return EMMC_CARD_STATE_STBY;
        case 4: return EMMC_CARD_STATE_TRAN;
        case 5: return EMMC_CARD_STATE_DATA;
        case 6: return EMMC_CARD_STATE_RCV;
        case 7: return EMMC_CARD_STATE_PRG;
        case 8: return EMMC_CARD_STATE_DIS;
        default: return EMMC_CARD_STATE_IDLE;
    }
}
