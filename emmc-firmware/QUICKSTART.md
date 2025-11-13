# eMMC固件开发工程 - 快速入门指南

## 5分钟快速上手

### 步骤1：获取代码

项目已创建在 `/workspace/emmc-firmware/` 目录下。

### 步骤2：查看项目结构

```bash
cd /workspace/emmc-firmware
ls -la
```

你将看到：
```
├── inc/           # 头文件
├── src/           # 源代码
├── test/          # 测试程序
├── docs/          # 文档
├── Makefile       # 构建配置
└── build.sh       # 构建脚本
```

### 步骤3：编译项目

**方式一：使用Make**
```bash
make clean
make
```

**方式二：使用构建脚本**
```bash
./build.sh lib
```

### 步骤4：编译测试程序

```bash
make test
```

或者：
```bash
./build.sh test
```

### 步骤5：运行测试

```bash
make run-test
```

或者直接执行：
```bash
./build/bin/test_emmc
```

---

## 第一个程序

创建文件 `my_first_emmc.c`：

```c
#include "emmc_driver.h"
#include <stdio.h>

int main(void)
{
    // 1. 配置参数
    emmc_config_t config = {
        .base_clock = 200000000,
        .timeout_clk = 48000,
        .bus_width = EMMC_BUS_WIDTH_4BIT,
        .voltage = EMMC_VOLTAGE_3V3,
        .dma_enabled = false,
        .high_speed_enabled = true
    };
    
    // 2. 初始化
    emmc_status_t status = emmc_driver_init(&config);
    if (status != EMMC_SUCCESS) {
        printf("初始化失败: %d\n", status);
        return -1;
    }
    printf("✓ eMMC初始化成功\n");
    
    // 3. 获取卡信息
    emmc_card_info_t info;
    emmc_driver_get_card_info(&info);
    printf("✓ 卡容量: %llu 字节 (%.2f GB)\n", 
           info.capacity, 
           (double)info.capacity / (1024*1024*1024));
    
    // 4. 读写测试
    uint8_t buffer[512] = {0};
    
    // 写入
    for (int i = 0; i < 512; i++) {
        buffer[i] = i & 0xFF;
    }
    status = emmc_driver_write_blocks(100, buffer, 1);
    printf("✓ 写入完成\n");
    
    // 读取
    uint8_t read_buf[512];
    status = emmc_driver_read_blocks(100, read_buf, 1);
    printf("✓ 读取完成\n");
    
    // 5. 清理
    emmc_driver_deinit();
    printf("✓ 清理完成\n");
    
    return 0;
}
```

编译运行：
```bash
gcc -I./inc my_first_emmc.c -L./build/bin -lemmc_firmware -o my_app
./my_app
```

---

## 常用操作

### 读取数据块

```c
uint8_t buffer[512];
uint32_t block_addr = 0;  // 块地址

// 读取单个块
emmc_status_t status = emmc_driver_read_blocks(block_addr, buffer, 1);

// 读取多个块
uint8_t large_buffer[512 * 10];
status = emmc_driver_read_blocks(block_addr, large_buffer, 10);
```

### 写入数据块

```c
uint8_t buffer[512];
// 准备数据...

// 写入单个块
emmc_status_t status = emmc_driver_write_blocks(block_addr, buffer, 1);

// 写入多个块
uint8_t large_buffer[512 * 10];
status = emmc_driver_write_blocks(block_addr, large_buffer, 10);
```

### 擦除数据块

```c
// 擦除块100到块199
emmc_status_t status = emmc_driver_erase_blocks(100, 199);
```

### 获取容量

```c
uint64_t capacity = emmc_driver_get_capacity();
printf("容量: %llu 字节\n", capacity);
```

---

## 错误处理示例

```c
emmc_status_t status = emmc_driver_read_blocks(addr, buf, count);

switch (status) {
    case EMMC_SUCCESS:
        printf("操作成功\n");
        break;
    case EMMC_ERROR_TIMEOUT:
        printf("超时错误，请检查硬件连接\n");
        break;
    case EMMC_ERROR_CRC:
        printf("CRC错误，数据可能损坏\n");
        break;
    case EMMC_ERROR_INVALID_PARAM:
        printf("参数错误\n");
        break;
    default:
        printf("未知错误: %d\n", status);
        break;
}
```

---

## 调试技巧

### 1. 启用调试模式

```bash
make DEBUG=1
```

### 2. 查看寄存器状态

在代码中添加：
```c
#include "emmc_sim.h"
emmc_sim_print_registers();
```

### 3. 添加日志

```c
printf("[DEBUG] 块地址: %u, 块数: %u\n", block_addr, block_count);
```

---

## 下一步

1. **阅读API文档**
   - 查看 `docs/API.md` 了解所有API

2. **了解架构**
   - 查看 `docs/ARCHITECTURE.md` 理解系统设计

3. **查看测试代码**
   - 参考 `test/test_emmc.c` 了解使用方法

4. **尝试修改**
   - 修改配置参数
   - 添加新功能
   - 优化性能

---

## 常见问题

**Q: 如何修改寄存器基地址？**  
A: 编辑 `inc/emmc_regs.h`，修改 `EMMC_BASE_ADDR` 宏定义。

**Q: 如何使用DMA？**  
A: 在配置中设置 `config.dma_enabled = true`。

**Q: 支持哪些总线宽度？**  
A: 支持1-bit、4-bit、8-bit三种模式。

**Q: 最高支持多少频率？**  
A: 理论支持到400MHz（HS400模式）。

**Q: 如何移植到实际硬件？**  
A: 参考README中的移植指南章节。

---

## 获取帮助

- 查看 `README.md` 获取完整说明
- 查看 `docs/` 目录下的详细文档
- 查看测试代码了解使用示例

---

**开始你的eMMC之旅！** 🚀
