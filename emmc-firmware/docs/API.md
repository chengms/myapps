# eMMC固件 API 参考手册

## 目录

1. [驱动层API](#驱动层api)
2. [HAL层API](#hal层api)
3. [数据类型](#数据类型)
4. [错误码](#错误码)
5. [使用示例](#使用示例)

---

## 驱动层API

驱动层提供高级接口供应用程序使用。

### emmc_driver_init

初始化eMMC驱动程序。

```c
emmc_status_t emmc_driver_init(const emmc_config_t* config);
```

**参数：**
- `config` - 配置参数指针

**返回值：**
- `EMMC_SUCCESS` - 初始化成功
- `EMMC_ERROR_INVALID_PARAM` - 参数无效
- `EMMC_ERROR_NO_CARD` - 未检测到卡
- `EMMC_ERROR_TIMEOUT` - 初始化超时

**示例：**
```c
emmc_config_t config = {
    .base_clock = 200000000,
    .timeout_clk = 48000,
    .bus_width = EMMC_BUS_WIDTH_4BIT,
    .voltage = EMMC_VOLTAGE_3V3,
    .dma_enabled = false,
    .high_speed_enabled = true
};

emmc_status_t status = emmc_driver_init(&config);
if (status != EMMC_SUCCESS) {
    printf("初始化失败: %d\n", status);
}
```

---

### emmc_driver_deinit

反初始化eMMC驱动程序，释放资源。

```c
emmc_status_t emmc_driver_deinit(void);
```

**返回值：**
- `EMMC_SUCCESS` - 反初始化成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化

---

### emmc_driver_get_card_info

获取eMMC卡的详细信息。

```c
emmc_status_t emmc_driver_get_card_info(emmc_card_info_t* card_info);
```

**参数：**
- `card_info` - 用于接收卡信息的结构体指针

**返回值：**
- `EMMC_SUCCESS` - 获取成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化
- `EMMC_ERROR_INVALID_PARAM` - 参数无效

**示例：**
```c
emmc_card_info_t info;
if (emmc_driver_get_card_info(&info) == EMMC_SUCCESS) {
    printf("容量: %llu 字节\n", info.capacity);
    printf("块大小: %u\n", info.block_size);
    printf("块数量: %u\n", info.block_count);
}
```

---

### emmc_driver_read_blocks

从eMMC读取数据块。

```c
emmc_status_t emmc_driver_read_blocks(uint32_t block_addr, 
                                      uint8_t* buffer, 
                                      uint32_t block_count);
```

**参数：**
- `block_addr` - 起始块地址
- `buffer` - 数据缓冲区指针
- `block_count` - 要读取的块数量

**返回值：**
- `EMMC_SUCCESS` - 读取成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化
- `EMMC_ERROR_INVALID_PARAM` - 参数无效
- `EMMC_ERROR_TIMEOUT` - 操作超时
- `EMMC_ERROR_CRC` - CRC校验错误

**注意事项：**
- 缓冲区大小必须 >= block_count * 512 字节
- 块地址不能超出卡容量范围

**示例：**
```c
uint8_t buffer[512];
emmc_status_t status = emmc_driver_read_blocks(100, buffer, 1);
if (status == EMMC_SUCCESS) {
    // 处理读取的数据
}
```

---

### emmc_driver_write_blocks

向eMMC写入数据块。

```c
emmc_status_t emmc_driver_write_blocks(uint32_t block_addr, 
                                       const uint8_t* buffer, 
                                       uint32_t block_count);
```

**参数：**
- `block_addr` - 起始块地址
- `buffer` - 要写入的数据缓冲区指针
- `block_count` - 要写入的块数量

**返回值：**
- `EMMC_SUCCESS` - 写入成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化
- `EMMC_ERROR_INVALID_PARAM` - 参数无效
- `EMMC_ERROR_WRITE_PROTECT` - 写保护
- `EMMC_ERROR_TIMEOUT` - 操作超时

**示例：**
```c
uint8_t data[512];
// 准备数据...
emmc_status_t status = emmc_driver_write_blocks(100, data, 1);
```

---

### emmc_driver_erase_blocks

擦除eMMC数据块。

```c
emmc_status_t emmc_driver_erase_blocks(uint32_t start_addr, uint32_t end_addr);
```

**参数：**
- `start_addr` - 起始块地址
- `end_addr` - 结束块地址（包含）

**返回值：**
- `EMMC_SUCCESS` - 擦除成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化
- `EMMC_ERROR_INVALID_PARAM` - 参数无效
- `EMMC_ERROR_TIMEOUT` - 操作超时

**注意事项：**
- 擦除后数据将被清除（通常为0xFF）
- 擦除操作可能需要较长时间

---

### emmc_driver_get_capacity

获取eMMC容量。

```c
uint64_t emmc_driver_get_capacity(void);
```

**返回值：**
- 容量大小（字节），如果未初始化返回0

**示例：**
```c
uint64_t capacity = emmc_driver_get_capacity();
printf("容量: %.2f GB\n", (double)capacity / (1024*1024*1024));
```

---

### emmc_driver_is_ready

检查eMMC是否就绪。

```c
bool emmc_driver_is_ready(void);
```

**返回值：**
- `true` - 就绪
- `false` - 未就绪

---

### emmc_driver_set_speed_mode

设置eMMC速度模式。

```c
emmc_status_t emmc_driver_set_speed_mode(emmc_speed_mode_t speed_mode);
```

**参数：**
- `speed_mode` - 速度模式

**返回值：**
- `EMMC_SUCCESS` - 设置成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化

---

### emmc_driver_set_bus_width

设置总线宽度。

```c
emmc_status_t emmc_driver_set_bus_width(emmc_bus_width_t bus_width);
```

**参数：**
- `bus_width` - 总线宽度

**返回值：**
- `EMMC_SUCCESS` - 设置成功
- `EMMC_ERROR_NOT_INIT` - 驱动未初始化

---

### emmc_driver_get_card_state

获取当前卡状态。

```c
emmc_card_state_t emmc_driver_get_card_state(void);
```

**返回值：**
- 卡状态枚举值

---

## HAL层API

HAL层提供底层硬件访问接口。

### emmc_hal_init

初始化HAL层。

```c
emmc_status_t emmc_hal_init(const emmc_config_t* config);
```

---

### emmc_hal_set_clock

设置时钟频率。

```c
emmc_status_t emmc_hal_set_clock(uint32_t freq_hz);
```

**参数：**
- `freq_hz` - 目标频率（Hz）

---

### emmc_hal_set_power

设置电源状态。

```c
emmc_status_t emmc_hal_set_power(bool on, emmc_voltage_t voltage);
```

**参数：**
- `on` - 电源开关
- `voltage` - 电压选择

---

### emmc_hal_send_command

发送命令到eMMC。

```c
emmc_status_t emmc_hal_send_command(emmc_cmd_t* cmd);
```

**参数：**
- `cmd` - 命令结构体指针

---

### emmc_hal_wait_command_done

等待命令完成。

```c
emmc_status_t emmc_hal_wait_command_done(uint32_t timeout_ms);
```

**参数：**
- `timeout_ms` - 超时时间（毫秒）

---

## 数据类型

### emmc_config_t

驱动配置结构体。

```c
typedef struct {
    uint32_t base_clock;            // 基础时钟频率(Hz)
    uint32_t timeout_clk;           // 超时时钟频率(KHz)
    emmc_bus_width_t bus_width;     // 总线宽度
    emmc_voltage_t voltage;         // 工作电压
    bool dma_enabled;               // DMA使能
    bool high_speed_enabled;        // 高速模式使能
} emmc_config_t;
```

---

### emmc_card_info_t

卡信息结构体。

```c
typedef struct {
    uint8_t  cid[16];               // 卡识别号
    uint8_t  csd[16];               // 卡特定数据
    uint8_t  ext_csd[512];          // 扩展CSD
    uint32_t rca;                   // 相对卡地址
    uint32_t ocr;                   // 操作条件寄存器
    uint64_t capacity;              // 容量（字节）
    uint32_t block_size;            // 块大小
    uint32_t block_count;           // 块数量
    emmc_bus_width_t bus_width;     // 总线宽度
    emmc_speed_mode_t speed_mode;   // 速度模式
    bool     is_initialized;        // 是否已初始化
} emmc_card_info_t;
```

---

### emmc_bus_width_t

总线宽度枚举。

```c
typedef enum {
    EMMC_BUS_WIDTH_1BIT = 0,
    EMMC_BUS_WIDTH_4BIT = 1,
    EMMC_BUS_WIDTH_8BIT = 2
} emmc_bus_width_t;
```

---

### emmc_speed_mode_t

速度模式枚举。

```c
typedef enum {
    EMMC_SPEED_MODE_LEGACY = 0,     // 传统模式 (最高26MHz)
    EMMC_SPEED_MODE_HIGH_SPEED,     // 高速模式 (最高52MHz)
    EMMC_SPEED_MODE_HS200,          // HS200模式 (最高200MHz)
    EMMC_SPEED_MODE_HS400,          // HS400模式 (最高400MHz)
} emmc_speed_mode_t;
```

---

### emmc_voltage_t

电压枚举。

```c
typedef enum {
    EMMC_VOLTAGE_1V8 = 0,
    EMMC_VOLTAGE_3V3 = 1
} emmc_voltage_t;
```

---

## 错误码

### emmc_status_t

状态码枚举。

```c
typedef enum {
    EMMC_SUCCESS = 0,           // 成功
    EMMC_ERROR_TIMEOUT,         // 超时错误
    EMMC_ERROR_CRC,             // CRC错误
    EMMC_ERROR_INVALID_PARAM,   // 无效参数
    EMMC_ERROR_NOT_INIT,        // 未初始化
    EMMC_ERROR_BUSY,            // 设备忙
    EMMC_ERROR_NO_CARD,         // 无卡
    EMMC_ERROR_COMM,            // 通信错误
    EMMC_ERROR_WRITE_PROTECT,   // 写保护
    EMMC_ERROR_UNKNOWN          // 未知错误
} emmc_status_t;
```

---

## 使用示例

### 完整的读写示例

```c
#include "emmc_driver.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    emmc_status_t status;
    uint8_t write_buf[512];
    uint8_t read_buf[512];
    
    // 1. 初始化驱动
    emmc_config_t config = {
        .base_clock = 200000000,
        .timeout_clk = 48000,
        .bus_width = EMMC_BUS_WIDTH_4BIT,
        .voltage = EMMC_VOLTAGE_3V3,
        .dma_enabled = false,
        .high_speed_enabled = true
    };
    
    status = emmc_driver_init(&config);
    if (status != EMMC_SUCCESS) {
        printf("初始化失败\n");
        return -1;
    }
    
    // 2. 获取卡信息
    emmc_card_info_t info;
    emmc_driver_get_card_info(&info);
    printf("容量: %llu 字节\n", info.capacity);
    
    // 3. 准备数据
    for (int i = 0; i < 512; i++) {
        write_buf[i] = i & 0xFF;
    }
    
    // 4. 写入数据
    status = emmc_driver_write_blocks(100, write_buf, 1);
    if (status != EMMC_SUCCESS) {
        printf("写入失败\n");
        return -1;
    }
    
    // 5. 读取数据
    status = emmc_driver_read_blocks(100, read_buf, 1);
    if (status != EMMC_SUCCESS) {
        printf("读取失败\n");
        return -1;
    }
    
    // 6. 验证数据
    if (memcmp(write_buf, read_buf, 512) == 0) {
        printf("读写验证成功\n");
    } else {
        printf("读写验证失败\n");
    }
    
    // 7. 清理
    emmc_driver_deinit();
    
    return 0;
}
```

---

**文档版本：** 1.0  
**最后更新：** 2025-11-13
