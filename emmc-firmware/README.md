# eMMC固件开发工程

## 项目简介

这是一个嵌入式存储eMMC（Embedded Multi-Media Card）的固件开发工程，提供了完整的eMMC控制器驱动实现和模拟环境。本项目适用于�MMC固件开发、驱动调试和功能验证。

### 主要特性

- ✅ **完整的寄存器定义**：基于JEDEC eMMC 5.1规范的控制器寄存器映射
- ✅ **HAL硬件抽象层**：提供底层硬件接口抽象
- ✅ **驱动核心实现**：支持卡初始化、读写、擦除等基本操作
- ✅ **模拟环境**：无需真实硬件即可进行开发和测试
- ✅ **测试程序**：完善的功能测试用例
- ✅ **多种传输模式**：支持单块/多块读写
- ✅ **可配置参数**：支持不同总线宽度、速度模式和电压

## 项目结构

```
emmc-firmware/
├── inc/                    # 头文件目录
│   ├── emmc_regs.h        # 寄存器定义
│   ├── emmc_types.h       # 类型定义
│   ├── emmc_hal.h         # HAL层接口
│   └── emmc_driver.h      # 驱动接口
├── src/                    # 源文件目录
│   ├── emmc_hal.c         # HAL层实现
│   ├── emmc_driver.c      # 驱动实现
│   └── emmc_sim.c         # 模拟器实现
├── test/                   # 测试目录
│   └── test_emmc.c        # 测试程序
├── build/                  # 编译输出目录
├── docs/                   # 文档目录
├── Makefile               # 编译配置
├── build.sh               # 构建脚本
├── .gitignore            # Git忽略配置
└── README.md             # 项目说明
```

## 快速开始

### 环境要求

- GCC编译器（推荐版本 >= 7.0）
- Make工具
- Linux/Unix系统（或WSL）

### 编译

#### 1. 编译静态库

```bash
make
```

或使用构建脚本：

```bash
./build.sh lib
```

#### 2. 编译测试程序

```bash
make test
```

或使用构建脚本：

```bash
./build.sh test
```

#### 3. 运行测试

```bash
make run-test
```

或直接执行：

```bash
./build/bin/test_emmc
```

#### 4. 清理编译文件

```bash
make clean
```

### 调试模式

使用DEBUG=1标志启用调试模式：

```bash
make DEBUG=1
```

或使用构建脚本：

```bash
./build.sh debug
```

## API使用说明

### 1. 初始化驱动

```c
#include "emmc_driver.h"

// 配置参数
emmc_config_t config = {
    .base_clock = 200000000,        // 200MHz基础时钟
    .timeout_clk = 48000,           // 48MHz超时时钟
    .bus_width = EMMC_BUS_WIDTH_4BIT,
    .voltage = EMMC_VOLTAGE_3V3,
    .dma_enabled = false,
    .high_speed_enabled = true
};

// 初始化
emmc_status_t status = emmc_driver_init(&config);
if (status != EMMC_SUCCESS) {
    // 处理错误
}
```

### 2. 读取数据块

```c
uint8_t buffer[512];
uint32_t block_addr = 100;

// 读取单个块
status = emmc_driver_read_blocks(block_addr, buffer, 1);

// 读取多个块
uint8_t multi_buffer[512 * 8];
status = emmc_driver_read_blocks(block_addr, multi_buffer, 8);
```

### 3. 写入数据块

```c
uint8_t buffer[512];
// 准备要写入的数据...

// 写入单个块
status = emmc_driver_write_blocks(block_addr, buffer, 1);

// 写入多个块
uint8_t multi_buffer[512 * 8];
status = emmc_driver_write_blocks(block_addr, multi_buffer, 8);
```

### 4. 擦除数据块

```c
uint32_t start_addr = 100;
uint32_t end_addr = 200;

status = emmc_driver_erase_blocks(start_addr, end_addr);
```

### 5. 获取卡信息

```c
emmc_card_info_t card_info;
status = emmc_driver_get_card_info(&card_info);

printf("容量: %llu 字节\n", card_info.capacity);
printf("块大小: %u\n", card_info.block_size);
printf("块数量: %u\n", card_info.block_count);
```

### 6. 反初始化

```c
status = emmc_driver_deinit();
```

## 寄存器说明

### 主要寄存器

| 寄存器名称 | 偏移地址 | 说明 |
|-----------|---------|------|
| EMMC_SYSADDR | 0x000 | DMA系统地址 |
| EMMC_BLKSIZE | 0x004 | 块大小配置 |
| EMMC_BLKCNT | 0x006 | 块计数 |
| EMMC_ARGUMENT | 0x008 | 命令参数 |
| EMMC_COMMAND | 0x00E | 命令寄存器 |
| EMMC_RESPONSE0-3 | 0x010-0x01C | 命令响应 |
| EMMC_PRESENT_STATE | 0x024 | 当前状态 |
| EMMC_HOST_CTRL | 0x028 | 主机控制 |
| EMMC_PWR_CTRL | 0x029 | 电源控制 |
| EMMC_CLOCK_CTRL | 0x02C | 时钟控制 |
| EMMC_INT_STATUS | 0x030 | 中断状态 |

详细寄存器定义请参考 `inc/emmc_regs.h`

## eMMC命令说明

### 基本命令

| 命令 | 索引 | 说明 |
|-----|------|------|
| CMD0 | 0 | GO_IDLE_STATE - 复位到空闲状态 |
| CMD1 | 1 | SEND_OP_COND - 发送操作条件 |
| CMD2 | 2 | ALL_SEND_CID - 获取CID |
| CMD3 | 3 | SET_RELATIVE_ADDR - 设置相对地址 |
| CMD7 | 7 | SELECT_CARD - 选择/取消选择卡 |
| CMD8 | 8 | SEND_EXT_CSD - 发送扩展CSD |
| CMD9 | 9 | SEND_CSD - 发送CSD |

### 数据传输命令

| 命令 | 索引 | 说明 |
|-----|------|------|
| CMD17 | 17 | READ_SINGLE_BLOCK - 读单块 |
| CMD18 | 18 | READ_MULTIPLE_BLOCK - 读多块 |
| CMD24 | 24 | WRITE_BLOCK - 写单块 |
| CMD25 | 25 | WRITE_MULTIPLE_BLOCK - 写多块 |

## 配置选项

### 总线宽度

- `EMMC_BUS_WIDTH_1BIT` - 1位数据线
- `EMMC_BUS_WIDTH_4BIT` - 4位数据线
- `EMMC_BUS_WIDTH_8BIT` - 8位数据线

### 速度模式

- `EMMC_SPEED_MODE_LEGACY` - 传统模式（最高26MHz）
- `EMMC_SPEED_MODE_HIGH_SPEED` - 高速模式（最高52MHz）
- `EMMC_SPEED_MODE_HS200` - HS200模式（最高200MHz）
- `EMMC_SPEED_MODE_HS400` - HS400模式（最高400MHz）

### 工作电压

- `EMMC_VOLTAGE_1V8` - 1.8V
- `EMMC_VOLTAGE_3V3` - 3.3V

## 错误码说明

| 错误码 | 说明 |
|-------|------|
| EMMC_SUCCESS | 操作成功 |
| EMMC_ERROR_TIMEOUT | 超时错误 |
| EMMC_ERROR_CRC | CRC校验错误 |
| EMMC_ERROR_INVALID_PARAM | 无效参数 |
| EMMC_ERROR_NOT_INIT | 驱动未初始化 |
| EMMC_ERROR_BUSY | 设备忙 |
| EMMC_ERROR_NO_CARD | 无卡 |
| EMMC_ERROR_COMM | 通信错误 |
| EMMC_ERROR_WRITE_PROTECT | 写保护 |

## 测试说明

测试程序包含以下测试用例：

1. **初始化测试** - 验证驱动初始化流程
2. **卡信息测试** - 获取并显示卡信息
3. **单块读写测试** - 测试单个块的读写操作
4. **多块读写测试** - 测试多个块的批量读写
5. **擦除测试** - 测试块擦除功能
6. **反初始化测试** - 验证资源清理

运行测试：

```bash
./build/bin/test_emmc
```

## 性能优化建议

1. **使用DMA传输** - 对于大量数据传输，启用DMA可显著提高性能
2. **多块操作** - 使用多块读写命令代替多次单块操作
3. **合理的时钟频率** - 根据实际需求选择合适的时钟频率
4. **总线宽度** - 使用8位数据线可获得最佳性能

## 移植指南

要将此驱动移植到实际硬件平台：

1. **修改基地址** - 在 `emmc_regs.h` 中修改 `EMMC_BASE_ADDR` 为实际的寄存器基地址

2. **实现延时函数** - 在 `emmc_hal.c` 中实现：
   - `emmc_hal_delay_ms()` - 毫秒延时
   - `emmc_hal_delay_us()` - 微秒延时
   - `emmc_hal_get_tick_ms()` - 获取系统时间戳

3. **配置中断** - 如果使用中断模式，需要实现中断处理函数

4. **DMA配置** - 如果使用DMA，需要根据平台配置DMA控制器

5. **电源管理** - 实现电源控制相关接口

## 注意事项

- ⚠️ 本项目中的寄存器地址和模拟实现仅用于开发测试
- ⚠️ 移植到实际硬件时需要根据芯片手册调整寄存器定义
- ⚠️ 实际使用时建议添加更完善的错误处理和日志记录
- ⚠️ 对于生产环境，建议增加更多的健壮性检查

## 版本历史

### v1.0 (2025-11-13)
- ✅ 初始版本发布
- ✅ 基本的驱动框架
- ✅ HAL层抽象
- ✅ 模拟环境支持
- ✅ 完整的测试用例

## 许可证

本项目采用MIT许可证。详见LICENSE文件。

## 贡献

欢迎提交问题报告和改进建议！

## 联系方式

如有问题或建议，请通过以下方式联系：

- 创建Issue
- 提交Pull Request

## 参考资料

- [JEDEC eMMC Standard](https://www.jedec.org/)
- [SD Host Controller Simplified Specification](https://www.sdcard.org/)
- 相关芯片厂商技术手册

---

**祝使用愉快！** 🚀
