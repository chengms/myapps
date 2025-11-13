# eMMC固件开发工程

这是一个嵌入式存储eMMC的固件开发项目，包含完整的寄存器模拟和驱动实现。

## 项目结构

```
emmc-firmware/
├── include/              # 头文件目录
│   ├── emmc_registers.h # eMMC寄存器定义
│   ├── emmc_driver.h    # eMMC驱动接口
│   └── hal.h            # 硬件抽象层接口
├── src/                  # 源文件目录
│   ├── emmc_driver.c    # eMMC驱动实现
│   ├── hal.c            # 硬件抽象层实现
│   └── main.c           # 主程序
├── config/              # 配置文件目录
│   └── config.h         # 项目配置
├── build/               # 构建输出目录（自动生成）
├── Makefile             # Make构建文件
├── CMakeLists.txt       # CMake构建文件
└── README.md            # 本文件
```

## 功能特性

### 寄存器模拟
- 完整的eMMC控制器寄存器定义
- 控制寄存器、状态寄存器、命令寄存器等
- 中断寄存器、时钟控制寄存器、电源控制寄存器
- 数据传输相关寄存器（FIFO、块大小、块计数等）

### 驱动功能
- eMMC初始化与反初始化
- 单块/多块读写操作
- 块擦除功能
- 命令发送与响应处理
- 时钟和总线宽度配置
- 中断处理支持

### 硬件抽象层
- 延时函数（微秒/毫秒级）
- 时间获取函数
- 可扩展的硬件接口

## 编译方法

### 使用Makefile

```bash
# 编译可执行文件
make

# 编译静态库
make lib

# 运行测试
make test

# 清理构建文件
make clean

# 查看帮助
make help
```

### 使用CMake

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
make

# 安装（可选）
make install
```

## 使用方法

### 基本初始化

```c
#include "emmc_driver.h"

emmc_config_t config = {
    .clock_freq = 25000000,  // 25MHz
    .bus_width = 4,          // 4位总线
    .dma_enable = false,
    .high_speed = false
};

emmc_error_t ret = emmc_init(&config);
if (ret == EMMC_OK) {
    // 初始化成功
}
```

### 读写操作

```c
uint8_t buffer[512];

// 写入单个块
emmc_write_block(0, buffer, 512);

// 读取单个块
emmc_read_block(0, buffer, 512);

// 写入多个块
emmc_write_blocks(0, 4, buffer, 512);

// 读取多个块
emmc_read_blocks(0, 4, buffer, 512);
```

### 获取设备信息

```c
emmc_device_t device;
emmc_get_device_info(&device);

printf("容量: %u MB\n", device.capacity / (1024 * 1024));
printf("块大小: %u 字节\n", device.block_size);
printf("块数量: %u\n", device.block_count);
```

## 寄存器说明

### 主要寄存器组

1. **控制寄存器组** (0x00-0x14)
   - 控制寄存器：软件复位、中断使能、DMA使能等
   - 状态寄存器：命令完成、数据完成、错误状态等
   - 中断寄存器：各种中断使能和状态
   - 时钟控制：时钟使能、分频器设置

2. **命令寄存器组** (0x18-0x2C)
   - 命令参数寄存器
   - 命令寄存器
   - 响应寄存器（4个）

3. **数据传输寄存器组** (0x30-0x48)
   - 数据超时、长度、控制寄存器
   - 数据地址寄存器
   - FIFO数据寄存器
   - 块大小和计数寄存器

4. **电源和特性寄存器** (0x4C-0x58)
   - 电源控制寄存器
   - 电压选择寄存器
   - 版本和能力寄存器

## 寄存器访问

寄存器通过宏定义进行访问：

```c
// 读取寄存器
uint32_t status = EMMC_READ_REG(EMMC_REG_STATUS);

// 写入寄存器
EMMC_WRITE_REG(EMMC_REG_CONTROL, value);

// 位操作
EMMC_SET_BITS(EMMC_REG_CONTROL, EMMC_CTRL_RESET);
EMMC_CLEAR_BITS(EMMC_REG_CONTROL, EMMC_CTRL_RESET);
```

## 配置说明

在 `config/config.h` 中可以配置：

- eMMC控制器基地址
- 系统时钟频率
- 默认时钟频率和总线宽度
- 超时时间
- 调试选项

## 注意事项

1. **寄存器模拟**：当前所有寄存器位置都是模拟的，实际使用时需要根据具体芯片手册调整基地址和偏移量。

2. **延时函数**：`hal.c` 中的延时函数是简化实现，实际使用时需要根据目标平台实现。

3. **中断处理**：中断处理函数需要根据实际硬件平台进行实现和注册。

4. **时间函数**：`hal_get_tick_ms()` 需要从系统定时器获取实际时间。

5. **内存映射**：寄存器访问假设内存映射I/O，实际使用时可能需要通过特定接口访问。

## 扩展开发

### 添加新功能

1. 在 `emmc_registers.h` 中添加新的寄存器定义
2. 在 `emmc_driver.h` 中添加新的API接口
3. 在 `emmc_driver.c` 中实现新功能
4. 更新文档和测试代码

### 移植到实际硬件

1. 修改 `emmc_registers.h` 中的基地址和寄存器偏移
2. 实现 `hal.c` 中的延时和时间函数
3. 根据实际硬件调整中断处理
4. 测试和验证功能

## 许可证

本项目仅供学习和开发使用。

## 作者

eMMC固件开发团队

## 更新日志

### v1.0.0 (2024)
- 初始版本
- 完整的寄存器定义和模拟
- 基础驱动功能实现
- 读写操作支持
