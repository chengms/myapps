/**
 * @file emmc_sim.c
 * @brief eMMC寄存器模拟实现
 * @version 1.0
 * @date 2025-11-13
 * 
 * 本文件提供eMMC控制器寄存器的内存模拟
 * 用于在没有真实硬件的情况下进行固件开发和测试
 */

#include "emmc_regs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 模拟的寄存器空间 */
static uint8_t g_emmc_reg_space[4096] = {0};

/* 模拟的存储空间 (32MB用于测试) */
#define SIM_STORAGE_SIZE    (32 * 1024 * 1024)
static uint8_t* g_emmc_storage = NULL;

/* 模拟状态 */
static bool g_sim_initialized = false;
static bool g_sim_card_present = true;

/**
 * @brief 初始化模拟器
 */
void emmc_sim_init(void)
{
    if (g_sim_initialized) {
        return;
    }
    
    /* 分配存储空间 */
    g_emmc_storage = (uint8_t*)malloc(SIM_STORAGE_SIZE);
    if (g_emmc_storage == NULL) {
        printf("警告: 无法分配模拟存储空间\n");
        return;
    }
    
    /* 初始化存储空间 */
    memset(g_emmc_storage, 0xFF, SIM_STORAGE_SIZE); /* 擦除状态为0xFF */
    
    /* 初始化寄存器默认值 */
    memset(g_emmc_reg_space, 0, sizeof(g_emmc_reg_space));
    
    /* 设置能力寄存器 */
    emmc_regs_t* regs = (emmc_regs_t*)g_emmc_reg_space;
    regs->capabilities[0] = 0x05E80080; /* 支持SDMA, 高速, 3.3V等 */
    regs->capabilities[1] = 0x00000007; /* 最大块长度等 */
    
    /* 设置当前状态 - 卡插入且稳定 */
    regs->present_state = EMMC_STATE_CARD_INSERT | EMMC_STATE_CARD_STABLE;
    
    g_sim_initialized = true;
    printf("eMMC模拟器已初始化 (模拟存储: %d MB)\n", SIM_STORAGE_SIZE / (1024*1024));
}

/**
 * @brief 清理模拟器
 */
void emmc_sim_cleanup(void)
{
    if (g_emmc_storage != NULL) {
        free(g_emmc_storage);
        g_emmc_storage = NULL;
    }
    
    g_sim_initialized = false;
}

/**
 * @brief 读取模拟数据
 */
void emmc_sim_read_data(uint32_t addr, uint8_t* buffer, uint32_t size)
{
    if (!g_sim_initialized || g_emmc_storage == NULL) {
        return;
    }
    
    if (addr + size > SIM_STORAGE_SIZE) {
        printf("警告: 读取地址超出范围\n");
        return;
    }
    
    memcpy(buffer, &g_emmc_storage[addr], size);
}

/**
 * @brief 写入模拟数据
 */
void emmc_sim_write_data(uint32_t addr, const uint8_t* buffer, uint32_t size)
{
    if (!g_sim_initialized || g_emmc_storage == NULL) {
        return;
    }
    
    if (addr + size > SIM_STORAGE_SIZE) {
        printf("警告: 写入地址超出范围\n");
        return;
    }
    
    memcpy(&g_emmc_storage[addr], buffer, size);
}

/**
 * @brief 擦除模拟数据
 */
void emmc_sim_erase_data(uint32_t addr, uint32_t size)
{
    if (!g_sim_initialized || g_emmc_storage == NULL) {
        return;
    }
    
    if (addr + size > SIM_STORAGE_SIZE) {
        printf("警告: 擦除地址超出范围\n");
        return;
    }
    
    memset(&g_emmc_storage[addr], 0xFF, size);
}

/**
 * @brief 设置卡插入状态
 */
void emmc_sim_set_card_present(bool present)
{
    g_sim_card_present = present;
    
    emmc_regs_t* regs = (emmc_regs_t*)g_emmc_reg_space;
    if (present) {
        regs->present_state |= EMMC_STATE_CARD_INSERT | EMMC_STATE_CARD_STABLE;
        regs->int_status |= EMMC_INT_CARD_INSERT;
    } else {
        regs->present_state &= ~(EMMC_STATE_CARD_INSERT | EMMC_STATE_CARD_STABLE);
        regs->int_status |= EMMC_INT_CARD_REMOVE;
    }
}

/**
 * @brief 获取模拟存储大小
 */
uint32_t emmc_sim_get_storage_size(void)
{
    return SIM_STORAGE_SIZE;
}

/**
 * @brief 打印寄存器状态（调试用）
 */
void emmc_sim_print_registers(void)
{
    if (!g_sim_initialized) {
        printf("模拟器未初始化\n");
        return;
    }
    
    emmc_regs_t* regs = (emmc_regs_t*)g_emmc_reg_space;
    
    printf("\n========== eMMC寄存器状态 ==========\n");
    printf("当前状态:     0x%08X\n", regs->present_state);
    printf("中断状态:     0x%08X\n", regs->int_status);
    printf("中断使能:     0x%08X\n", regs->int_status_en);
    printf("主机控制:     0x%02X\n", regs->host_ctrl);
    printf("电源控制:     0x%02X\n", regs->pwr_ctrl);
    printf("时钟控制:     0x%04X\n", regs->clock_ctrl);
    printf("能力寄存器0:  0x%08X\n", regs->capabilities[0]);
    printf("能力寄存器1:  0x%08X\n", regs->capabilities[1]);
    printf("====================================\n\n");
}
