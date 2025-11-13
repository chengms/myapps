# eMMC寄存器详细说明

本文档详细说明eMMC控制器的所有寄存器定义和使用方法。

## 寄存器映射表

| 偏移地址 | 寄存器名称 | 大小 | 说明 |
|---------|-----------|------|------|
| 0x00 | CONTROL | 32位 | 控制寄存器 |
| 0x04 | STATUS | 32位 | 状态寄存器 |
| 0x08 | INT_ENABLE | 32位 | 中断使能寄存器 |
| 0x0C | INT_STATUS | 32位 | 中断状态寄存器 |
| 0x10 | CLOCK_CONTROL | 32位 | 时钟控制寄存器 |
| 0x14 | TIMEOUT | 32位 | 超时寄存器 |
| 0x18 | CMD_ARG | 32位 | 命令参数寄存器 |
| 0x1C | CMD | 32位 | 命令寄存器 |
| 0x20 | RESP0 | 32位 | 响应寄存器0 |
| 0x24 | RESP1 | 32位 | 响应寄存器1 |
| 0x28 | RESP2 | 32位 | 响应寄存器2 |
| 0x2C | RESP3 | 32位 | 响应寄存器3 |
| 0x30 | DATA_TIMEOUT | 32位 | 数据超时寄存器 |
| 0x34 | DATA_LENGTH | 32位 | 数据长度寄存器 |
| 0x38 | DATA_CONTROL | 32位 | 数据控制寄存器 |
| 0x3C | DATA_ADDR | 32位 | 数据地址寄存器 |
| 0x40 | FIFO | 32位 | FIFO数据寄存器 |
| 0x44 | BLOCK_SIZE | 32位 | 块大小寄存器 |
| 0x48 | BLOCK_COUNT | 32位 | 块计数寄存器 |
| 0x4C | POWER_CONTROL | 32位 | 电源控制寄存器 |
| 0x50 | VOLTAGE_SELECT | 32位 | 电压选择寄存器 |
| 0x54 | VERSION | 32位 | 版本寄存器 |
| 0x58 | CAPABILITIES | 32位 | 能力寄存器 |

## 控制寄存器 (CONTROL, 0x00)

控制eMMC控制器的基本操作。

| 位 | 名称 | 说明 |
|----|------|------|
| 0 | RESET | 软件复位，写1复位控制器 |
| 1 | INT_ENABLE | 中断使能 |
| 2 | DMA_ENABLE | DMA使能 |
| 3 | READ_WAIT | 读等待使能 |
| 4 | SEND_IRQ_RESP | 发送IRQ响应 |
| 5 | ABORT_READ | 中止读操作 |
| 6 | SEND_CCSD | 发送CCSD |
| 7 | SEND_AUTO_STOP | 发送自动停止 |
| 8 | 4BIT_MODE | 4位数据总线模式 |
| 9 | 8BIT_MODE | 8位数据总线模式 |
| 10 | HIGH_SPEED | 高速模式 |
| 11 | DDR_MODE | DDR模式 |

## 状态寄存器 (STATUS, 0x04)

反映eMMC控制器的当前状态。

| 位 | 名称 | 说明 |
|----|------|------|
| 0 | CMD_COMPLETE | 命令完成 |
| 1 | DATA_COMPLETE | 数据完成 |
| 2 | BLOCK_GAP | 块间隙 |
| 3 | TRANSFER_COMPLETE | 传输完成 |
| 4 | WRITE_PROTECT | 写保护 |
| 5 | INSERTED | 卡插入 |
| 6 | REMOVED | 卡移除 |
| 7 | CARD_PRESENT | 卡存在 |
| 8 | CMD_ACTIVE | 命令激活 |
| 9 | DATA_ACTIVE | 数据激活 |
| 10 | FIFO_EMPTY | FIFO空 |
| 11 | FIFO_FULL | FIFO满 |
| 12 | BUSY | 忙状态 |
| 13 | ERROR | 错误状态 |

## 命令寄存器 (CMD, 0x1C)

配置和发送eMMC命令。

| 位 | 名称 | 说明 |
|----|------|------|
| 0-5 | INDEX | 命令索引 (0-63) |
| 6 | RESPONSE | 需要响应 |
| 7 | LONG_RESPONSE | 长响应 (136位) |
| 8 | CHECK_RESP_CRC | 检查响应CRC |
| 9 | DATA_PRESENT | 数据存在 |
| 10 | READ | 读操作 |
| 11 | WRITE | 写操作 |
| 12 | STREAM | 流模式 |
| 13 | MULTI_BLOCK | 多块操作 |
| 14 | STOP_TRANSFER | 停止传输 |
| 15 | WAIT_PRVDATA | 等待前一个数据 |
| 16 | SEND_AUTO_STOP | 发送自动停止 |
| 17 | ABORT_CMD | 中止命令 |

## 时钟控制寄存器 (CLOCK_CONTROL, 0x10)

控制eMMC时钟频率。

| 位 | 名称 | 说明 |
|----|------|------|
| 0 | ENABLE | 时钟使能 |
| 8-15 | DIVIDER | 时钟分频器 (2-256) |
| 16-17 | SELECT | 时钟源选择 |

计算公式：`输出频率 = 系统时钟 / 分频器`

## 数据控制寄存器 (DATA_CONTROL, 0x38)

控制数据传输。

| 位 | 名称 | 说明 |
|----|------|------|
| 0 | ENABLE | 数据使能 |
| 1 | DIRECTION | 方向：0=读，1=写 |
| 2 | MODE | 模式：0=块，1=流 |
| 3 | DMA_ENABLE | DMA使能 |

## 使用示例

### 初始化eMMC

```c
// 1. 复位控制器
EMMC_SET_BITS(EMMC_REG_CONTROL, EMMC_CTRL_RESET);

// 2. 上电
EMMC_WRITE_REG(EMMC_REG_POWER_CONTROL, EMMC_PWR_ON | (0x7 << EMMC_PWR_VOLTAGE_SHIFT));

// 3. 设置时钟（25MHz，假设系统时钟100MHz，分频器=4）
EMMC_WRITE_REG(EMMC_REG_CLOCK_CONTROL, EMMC_CLK_ENABLE | (4 << EMMC_CLK_DIVIDER_SHIFT));

// 4. 使能中断
EMMC_WRITE_REG(EMMC_REG_INT_ENABLE, EMMC_INT_CMD_COMPLETE | EMMC_INT_DATA_COMPLETE);
```

### 发送命令

```c
// 1. 设置命令参数
EMMC_WRITE_REG(EMMC_REG_CMD_ARG, 0x12345678);

// 2. 配置命令寄存器
uint32_t cmd_reg = EMMC_CMD_READ_SINGLE_BLOCK;
cmd_reg |= EMMC_CMD_RESPONSE;
cmd_reg |= EMMC_CMD_CHECK_RESP_CRC;
cmd_reg |= EMMC_CMD_DATA_PRESENT;
cmd_reg |= EMMC_CMD_READ;

// 3. 发送命令
EMMC_WRITE_REG(EMMC_REG_CMD, cmd_reg);

// 4. 等待命令完成
while (!(EMMC_READ_REG(EMMC_REG_STATUS) & EMMC_STAT_CMD_COMPLETE));

// 5. 读取响应
uint32_t response = EMMC_READ_REG(EMMC_REG_RESP0);
```

### 数据传输

```c
// 读取数据
// 1. 设置块大小和数量
EMMC_WRITE_REG(EMMC_REG_BLOCK_SIZE, 512);
EMMC_WRITE_REG(EMMC_REG_BLOCK_COUNT, 1);

// 2. 设置数据控制
EMMC_WRITE_REG(EMMC_REG_DATA_CONTROL, 
               EMMC_DATA_CTRL_ENABLE | (0 << EMMC_DATA_CTRL_DIRECTION));

// 3. 从FIFO读取数据
for (int i = 0; i < 128; i++) {  // 512字节 = 128个32位字
    uint32_t data = EMMC_READ_REG(EMMC_REG_FIFO);
    buffer[i * 4 + 0] = (data >> 0) & 0xFF;
    buffer[i * 4 + 1] = (data >> 8) & 0xFF;
    buffer[i * 4 + 2] = (data >> 16) & 0xFF;
    buffer[i * 4 + 3] = (data >> 24) & 0xFF;
}
```

## 注意事项

1. **寄存器访问**：所有寄存器访问都是32位对齐的，使用volatile关键字确保不被编译器优化。

2. **命令序列**：发送命令前必须清除命令完成中断标志，命令完成后检查状态寄存器。

3. **FIFO访问**：FIFO是32位宽度的，读取/写入数据时需要注意字节序。

4. **超时处理**：所有操作都应该有超时机制，避免系统挂起。

5. **中断处理**：使用中断方式可以提高效率，但需要正确处理中断标志。

6. **电源管理**：上电后需要等待电源稳定（通常10ms以上）才能进行操作。
