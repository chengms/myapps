/**
 * @file test_emmc.c
 * @brief eMMC驱动测试程序
 * @version 1.0
 * @date 2025-11-13
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "emmc_driver.h"

/* 测试缓冲区大小 */
#define TEST_BUFFER_SIZE    (512 * 8)  /* 8个块 */

/* 测试函数声明 */
static void test_init(void);
static void test_card_info(void);
static void test_read_write(void);
static void test_multi_block_rw(void);
static void test_erase(void);
static void print_buffer_hex(const uint8_t* buffer, uint32_t size);
static const char* status_to_string(emmc_status_t status);

/**
 * @brief 主测试函数
 */
int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf("  eMMC固件驱动测试程序\n");
    printf("  版本: 1.0\n");
    printf("  日期: 2025-11-13\n");
    printf("========================================\n\n");
    
    /* 测试1: 初始化 */
    printf("[测试1] eMMC驱动初始化测试\n");
    test_init();
    
    /* 测试2: 获取卡信息 */
    printf("\n[测试2] 获取卡信息测试\n");
    test_card_info();
    
    /* 测试3: 单块读写测试 */
    printf("\n[测试3] 单块读写测试\n");
    test_read_write();
    
    /* 测试4: 多块读写测试 */
    printf("\n[测试4] 多块读写测试\n");
    test_multi_block_rw();
    
    /* 测试5: 擦除测试 */
    printf("\n[测试5] 块擦除测试\n");
    test_erase();
    
    /* 反初始化 */
    printf("\n[测试6] 驱动反初始化测试\n");
    emmc_status_t status = emmc_driver_deinit();
    printf("反初始化结果: %s\n", status_to_string(status));
    
    printf("\n========================================\n");
    printf("  所有测试完成!\n");
    printf("========================================\n\n");
    
    return 0;
}

/**
 * @brief 测试初始化
 */
static void test_init(void)
{
    emmc_config_t config = {
        .base_clock = 200000000,        /* 200MHz基础时钟 */
        .timeout_clk = 48000,           /* 48MHz超时时钟 */
        .bus_width = EMMC_BUS_WIDTH_4BIT,
        .voltage = EMMC_VOLTAGE_3V3,
        .dma_enabled = false,           /* 暂不使用DMA */
        .high_speed_enabled = true
    };
    
    emmc_status_t status = emmc_driver_init(&config);
    printf("初始化状态: %s\n", status_to_string(status));
    
    if (status == EMMC_SUCCESS) {
        printf("✓ eMMC驱动初始化成功\n");
        printf("  - 基础时钟: %u Hz\n", config.base_clock);
        printf("  - 总线宽度: %d位\n", (config.bus_width == EMMC_BUS_WIDTH_1BIT) ? 1 : 
                                      (config.bus_width == EMMC_BUS_WIDTH_4BIT) ? 4 : 8);
        printf("  - 工作电压: %s\n", (config.voltage == EMMC_VOLTAGE_1V8) ? "1.8V" : "3.3V");
        printf("  - 高速模式: %s\n", config.high_speed_enabled ? "启用" : "禁用");
    } else {
        printf("✗ eMMC驱动初始化失败\n");
    }
}

/**
 * @brief 测试获取卡信息
 */
static void test_card_info(void)
{
    emmc_card_info_t card_info;
    emmc_status_t status = emmc_driver_get_card_info(&card_info);
    
    printf("获取卡信息: %s\n", status_to_string(status));
    
    if (status == EMMC_SUCCESS) {
        printf("✓ 卡信息获取成功\n");
        printf("  - RCA: 0x%04X\n", card_info.rca);
        printf("  - OCR: 0x%08X\n", card_info.ocr);
        printf("  - 容量: %llu 字节 (%.2f GB)\n", 
               card_info.capacity, 
               (double)card_info.capacity / (1024*1024*1024));
        printf("  - 块大小: %u 字节\n", card_info.block_size);
        printf("  - 块数量: %u\n", card_info.block_count);
        printf("  - 总线宽度: %d位\n", 
               (card_info.bus_width == EMMC_BUS_WIDTH_1BIT) ? 1 :
               (card_info.bus_width == EMMC_BUS_WIDTH_4BIT) ? 4 : 8);
        
        /* 打印CID */
        printf("  - CID: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", card_info.cid[i]);
        }
        printf("\n");
        
        /* 打印CSD */
        printf("  - CSD: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", card_info.csd[i]);
        }
        printf("\n");
    } else {
        printf("✗ 卡信息获取失败\n");
    }
}

/**
 * @brief 测试单块读写
 */
static void test_read_write(void)
{
    uint8_t write_buffer[512];
    uint8_t read_buffer[512];
    emmc_status_t status;
    uint32_t test_addr = 100; /* 测试块地址 */
    
    /* 准备测试数据 */
    for (int i = 0; i < 512; i++) {
        write_buffer[i] = i & 0xFF;
    }
    
    /* 写入单个块 */
    printf("写入块地址 %u...\n", test_addr);
    status = emmc_driver_write_blocks(test_addr, write_buffer, 1);
    printf("写入状态: %s\n", status_to_string(status));
    
    if (status != EMMC_SUCCESS) {
        printf("✗ 单块写入失败\n");
        return;
    }
    
    /* 清空读缓冲区 */
    memset(read_buffer, 0, 512);
    
    /* 读取单个块 */
    printf("读取块地址 %u...\n", test_addr);
    status = emmc_driver_read_blocks(test_addr, read_buffer, 1);
    printf("读取状态: %s\n", status_to_string(status));
    
    if (status != EMMC_SUCCESS) {
        printf("✗ 单块读取失败\n");
        return;
    }
    
    /* 比较数据 */
    int errors = 0;
    for (int i = 0; i < 512; i++) {
        if (write_buffer[i] != read_buffer[i]) {
            errors++;
        }
    }
    
    if (errors == 0) {
        printf("✓ 单块读写测试通过 (数据完全匹配)\n");
    } else {
        printf("✗ 单块读写测试失败 (发现 %d 个错误)\n", errors);
    }
    
    /* 打印部分数据 */
    printf("写入数据前16字节: ");
    print_buffer_hex(write_buffer, 16);
    printf("读取数据前16字节: ");
    print_buffer_hex(read_buffer, 16);
}

/**
 * @brief 测试多块读写
 */
static void test_multi_block_rw(void)
{
    uint8_t* write_buffer = (uint8_t*)malloc(TEST_BUFFER_SIZE);
    uint8_t* read_buffer = (uint8_t*)malloc(TEST_BUFFER_SIZE);
    emmc_status_t status;
    uint32_t test_addr = 200; /* 测试块地址 */
    uint32_t block_count = 8;  /* 8个块 */
    
    if (write_buffer == NULL || read_buffer == NULL) {
        printf("✗ 内存分配失败\n");
        free(write_buffer);
        free(read_buffer);
        return;
    }
    
    /* 准备测试数据 */
    for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
        write_buffer[i] = (i * 3 + 0x5A) & 0xFF;
    }
    
    /* 写入多个块 */
    printf("写入 %u 个块，起始地址 %u...\n", block_count, test_addr);
    status = emmc_driver_write_blocks(test_addr, write_buffer, block_count);
    printf("写入状态: %s\n", status_to_string(status));
    
    if (status != EMMC_SUCCESS) {
        printf("✗ 多块写入失败\n");
        free(write_buffer);
        free(read_buffer);
        return;
    }
    
    /* 清空读缓冲区 */
    memset(read_buffer, 0, TEST_BUFFER_SIZE);
    
    /* 读取多个块 */
    printf("读取 %u 个块，起始地址 %u...\n", block_count, test_addr);
    status = emmc_driver_read_blocks(test_addr, read_buffer, block_count);
    printf("读取状态: %s\n", status_to_string(status));
    
    if (status != EMMC_SUCCESS) {
        printf("✗ 多块读取失败\n");
        free(write_buffer);
        free(read_buffer);
        return;
    }
    
    /* 比较数据 */
    int errors = 0;
    for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
        if (write_buffer[i] != read_buffer[i]) {
            errors++;
        }
    }
    
    if (errors == 0) {
        printf("✓ 多块读写测试通过 (数据完全匹配)\n");
    } else {
        printf("✗ 多块读写测试失败 (发现 %d 个错误)\n", errors);
    }
    
    /* 打印部分数据 */
    printf("写入数据前16字节: ");
    print_buffer_hex(write_buffer, 16);
    printf("读取数据前16字节: ");
    print_buffer_hex(read_buffer, 16);
    
    free(write_buffer);
    free(read_buffer);
}

/**
 * @brief 测试擦除
 */
static void test_erase(void)
{
    uint32_t start_addr = 300;
    uint32_t end_addr = 305;
    
    printf("擦除块范围: %u - %u\n", start_addr, end_addr);
    emmc_status_t status = emmc_driver_erase_blocks(start_addr, end_addr);
    printf("擦除状态: %s\n", status_to_string(status));
    
    if (status == EMMC_SUCCESS) {
        printf("✓ 块擦除测试通过\n");
    } else {
        printf("✗ 块擦除测试失败\n");
    }
}

/**
 * @brief 打印缓冲区（十六进制）
 */
static void print_buffer_hex(const uint8_t* buffer, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

/**
 * @brief 状态码转字符串
 */
static const char* status_to_string(emmc_status_t status)
{
    switch (status) {
        case EMMC_SUCCESS:              return "成功";
        case EMMC_ERROR_TIMEOUT:        return "超时错误";
        case EMMC_ERROR_CRC:            return "CRC错误";
        case EMMC_ERROR_INVALID_PARAM:  return "无效参数";
        case EMMC_ERROR_NOT_INIT:       return "未初始化";
        case EMMC_ERROR_BUSY:           return "设备忙";
        case EMMC_ERROR_NO_CARD:        return "无卡";
        case EMMC_ERROR_COMM:           return "通信错误";
        case EMMC_ERROR_WRITE_PROTECT:  return "写保护";
        default:                        return "未知错误";
    }
}
