/**
 * @file main.c
 * @brief eMMC固件主程序
 * @description eMMC固件开发的示例主程序
 */

#include <stdio.h>
#include <string.h>
#include "emmc_driver.h"
#include "hal.h"

/* ==================== 测试函数 ==================== */

static void test_emmc_init(void)
{
    emmc_config_t config = {
        .clock_freq = 25000000,  /* 25MHz */
        .bus_width = 4,          /* 4位总线 */
        .dma_enable = false,
        .high_speed = false
    };
    
    printf("初始化eMMC控制器...\n");
    emmc_error_t ret = emmc_init(&config);
    
    if (ret == EMMC_OK) {
        printf("eMMC初始化成功\n");
    } else {
        printf("eMMC初始化失败，错误代码: %d\n", ret);
        return;
    }
    
    /* 获取设备信息 */
    emmc_device_t device;
    ret = emmc_get_device_info(&device);
    
    if (ret == EMMC_OK) {
        printf("设备信息:\n");
        printf("  容量: %u MB\n", device.capacity / (1024 * 1024));
        printf("  块大小: %u 字节\n", device.block_size);
        printf("  块数量: %u\n", device.block_count);
        printf("  高容量: %s\n", device.high_capacity ? "是" : "否");
        printf("  RCA: 0x%04X\n", device.rca);
    }
}

static void test_emmc_read_write(void)
{
    uint8_t write_buffer[512];
    uint8_t read_buffer[512];
    
    /* 准备测试数据 */
    for (int i = 0; i < 512; i++) {
        write_buffer[i] = (uint8_t)(i & 0xFF);
    }
    
    printf("\n测试读写操作...\n");
    
    /* 写入测试 */
    printf("写入块0...\n");
    emmc_error_t ret = emmc_write_block(0, write_buffer, 512);
    if (ret != EMMC_OK) {
        printf("写入失败，错误代码: %d\n", ret);
        return;
    }
    printf("写入成功\n");
    
    /* 读取测试 */
    printf("读取块0...\n");
    memset(read_buffer, 0, 512);
    ret = emmc_read_block(0, read_buffer, 512);
    if (ret != EMMC_OK) {
        printf("读取失败，错误代码: %d\n", ret);
        return;
    }
    printf("读取成功\n");
    
    /* 验证数据 */
    if (memcmp(write_buffer, read_buffer, 512) == 0) {
        printf("数据验证成功\n");
    } else {
        printf("数据验证失败\n");
    }
}

static void test_emmc_multiple_blocks(void)
{
    uint8_t write_buffer[512 * 4];
    uint8_t read_buffer[512 * 4];
    
    /* 准备测试数据 */
    for (size_t i = 0; i < sizeof(write_buffer); i++) {
        write_buffer[i] = (uint8_t)(0xAA ^ (i & 0xFF));
    }
    
    printf("\n测试多块读写操作...\n");
    
    /* 写入4个块 */
    printf("写入块1-4...\n");
    emmc_error_t ret = emmc_write_blocks(1, 4, write_buffer, 512);
    if (ret != EMMC_OK) {
        printf("写入失败，错误代码: %d\n", ret);
        return;
    }
    printf("写入成功\n");
    
    /* 读取4个块 */
    printf("读取块1-4...\n");
    memset(read_buffer, 0, sizeof(read_buffer));
    ret = emmc_read_blocks(1, 4, read_buffer, 512);
    if (ret != EMMC_OK) {
        printf("读取失败，错误代码: %d\n", ret);
        return;
    }
    printf("读取成功\n");
    
    /* 验证数据 */
    if (memcmp(write_buffer, read_buffer, sizeof(write_buffer)) == 0) {
        printf("数据验证成功\n");
    } else {
        printf("数据验证失败\n");
    }
}

/* ==================== 主函数 ==================== */

int main(void)
{
    printf("========================================\n");
    printf("eMMC固件开发测试程序\n");
    printf("========================================\n\n");
    
    /* 初始化测试 */
    test_emmc_init();
    
    /* 读写测试 */
    test_emmc_read_write();
    
    /* 多块读写测试 */
    test_emmc_multiple_blocks();
    
    printf("\n========================================\n");
    printf("测试完成\n");
    printf("========================================\n");
    
    return 0;
}
