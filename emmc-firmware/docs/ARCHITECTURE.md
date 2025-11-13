# eMMC固件架构设计文档

## 1. 系统架构

### 1.1 整体架构

```
┌─────────────────────────────────────┐
│     应用层 (Application Layer)      │
│  - 文件系统                          │
│  - 块设备管理                        │
└─────────────────┬───────────────────┘
                  │
┌─────────────────▼───────────────────┐
│   驱动层 (Driver Layer)              │
│  - emmc_driver.c                     │
│  - 卡初始化和识别                    │
│  - 块读写操作                        │
│  - 状态管理                          │
└─────────────────┬───────────────────┘
                  │
┌─────────────────▼───────────────────┐
│   HAL层 (Hardware Abstraction)       │
│  - emmc_hal.c                        │
│  - 寄存器操作                        │
│  - 命令发送                          │
│  - 数据传输                          │
└─────────────────┬───────────────────┘
                  │
┌─────────────────▼───────────────────┐
│   硬件层 (Hardware Layer)            │
│  - eMMC控制器寄存器                  │
│  - DMA控制器                         │
│  - 中断控制器                        │
└─────────────────────────────────────┘
```

### 1.2 模块说明

#### 1.2.1 驱动层 (Driver Layer)

**职责：**
- 提供高层API接口
- 实现eMMC协议逻辑
- 管理卡状态
- 处理数据传输

**主要文件：**
- `emmc_driver.h/c` - 驱动核心实现

**关键功能：**
```c
// 初始化和反初始化
emmc_driver_init()
emmc_driver_deinit()

// 数据操作
emmc_driver_read_blocks()
emmc_driver_write_blocks()
emmc_driver_erase_blocks()

// 信息查询
emmc_driver_get_card_info()
emmc_driver_get_capacity()
emmc_driver_is_ready()
```

#### 1.2.2 HAL层 (Hardware Abstraction Layer)

**职责：**
- 封装寄存器操作
- 提供平台无关接口
- 实现底层命令和数据传输

**主要文件：**
- `emmc_hal.h/c` - HAL层实现

**关键功能：**
```c
// 硬件初始化
emmc_hal_init()
emmc_hal_set_clock()
emmc_hal_set_power()

// 命令操作
emmc_hal_send_command()
emmc_hal_wait_command_done()

// 数据传输
emmc_hal_read_data()
emmc_hal_write_data()

// 中断管理
emmc_hal_enable_int()
emmc_hal_disable_int()
emmc_hal_get_int_status()
```

#### 1.2.3 寄存器定义层

**职责：**
- 定义所有寄存器地址
- 定义寄存器位域
- 提供访问宏

**主要文件：**
- `emmc_regs.h` - 寄存器定义
- `emmc_types.h` - 类型定义

## 2. 数据流

### 2.1 初始化流程

```
emmc_driver_init()
    ├── emmc_hal_init()
    │   ├── 复位控制器
    │   ├── 配置中断
    │   └── 设置超时
    ├── emmc_hal_set_power(ON)
    ├── emmc_hal_set_clock(400KHz)
    ├── emmc_card_identification()
    │   ├── CMD0: GO_IDLE
    │   └── CMD1: SEND_OP_COND (循环直到就绪)
    ├── emmc_card_init_sequence()
    │   ├── CMD2: ALL_SEND_CID
    │   ├── CMD3: SET_RELATIVE_ADDR
    │   └── CMD9: SEND_CSD
    ├── emmc_select_card()
    │   └── CMD7: SELECT_CARD
    ├── emmc_hal_set_clock(52MHz)
    └── emmc_hal_set_bus_width(4-bit/8-bit)
```

### 2.2 读数据流程

```
emmc_driver_read_blocks()
    ├── 检查参数有效性
    ├── 配置块大小和块数
    ├── 设置传输模式
    │   ├── 数据方向：读
    │   ├── 块计数使能
    │   └── 多块模式（如需要）
    ├── 发送读命令
    │   ├── CMD17 (单块)
    │   └── CMD18 (多块)
    ├── emmc_hal_wait_command_done()
    ├── emmc_hal_read_data()
    │   ├── 等待缓冲区就绪
    │   ├── 从DATA_PORT读取
    │   └── 等待传输完成
    └── 返回状态
```

### 2.3 写数据流程

```
emmc_driver_write_blocks()
    ├── 检查参数有效性
    ├── 配置块大小和块数
    ├── 设置传输模式
    │   ├── 数据方向：写
    │   ├── 块计数使能
    │   └── 多块模式（如需要）
    ├── 发送写命令
    │   ├── CMD24 (单块)
    │   └── CMD25 (多块)
    ├── emmc_hal_wait_command_done()
    ├── emmc_hal_write_data()
    │   ├── 等待缓冲区就绪
    │   ├── 写入DATA_PORT
    │   └── 等待传输完成
    └── 返回状态
```

## 3. 状态机

### 3.1 卡状态转换

```
        IDLE
         │
         │ CMD1
         ▼
       READY
         │
         │ CMD2
         ▼
       IDENT
         │
         │ CMD3
         ▼
       STBY ◄──────┐
         │         │
         │ CMD7    │ CMD7
         ▼         │ (deselect)
       TRAN ───────┘
         │
         │ CMD17/18/24/25
         ▼
       DATA
         │
         │ 传输完成
         ▼
       TRAN
```

### 3.2 驱动状态

```c
typedef enum {
    DRIVER_STATE_UNINITIALIZED,
    DRIVER_STATE_INITIALIZING,
    DRIVER_STATE_READY,
    DRIVER_STATE_BUSY,
    DRIVER_STATE_ERROR
} driver_state_t;
```

## 4. 中断处理

### 4.1 中断源

- 命令完成中断
- 数据传输完成中断
- DMA中断
- 错误中断（超时、CRC等）
- 卡插入/移除中断

### 4.2 中断处理流程

```
IRQ Handler
    ├── 读取中断状态
    ├── 判断中断类型
    ├── 处理中断
    │   ├── 命令完成
    │   ├── 数据传输完成
    │   ├── 错误处理
    │   └── 卡状态变化
    ├── 清除中断标志
    └── 返回
```

## 5. DMA传输

### 5.1 SDMA模式

```
配置DMA
    ├── 设置系统地址寄存器
    ├── 使能DMA
    └── 启动传输
        ├── 硬件自动传输
        ├── DMA中断
        └── 更新地址继续
```

### 5.2 ADMA2模式

```
准备描述符表
    ├── 描述符1：传输N字节
    ├── 描述符2：传输M字节
    └── 描述符N：结束标志

配置ADMA
    ├── 设置描述符表地址
    ├── 选择ADMA2模式
    └── 启动传输
```

## 6. 错误处理

### 6.1 错误类型

| 错误类型 | 检测方式 | 恢复策略 |
|---------|---------|---------|
| 超时错误 | 中断状态 | 重试/复位 |
| CRC错误 | 中断状态 | 重试 |
| 通信错误 | 中断状态 | 复位命令线 |
| 参数错误 | 软件检查 | 返回错误 |

### 6.2 错误恢复流程

```
检测到错误
    ├── 记录错误信息
    ├── 停止当前操作
    ├── 复位相应部分
    │   ├── 复位CMD线
    │   ├── 复位DAT线
    │   └── 复位全部
    ├── 重新初始化（如需要）
    └── 返回错误码
```

## 7. 性能优化

### 7.1 优化策略

1. **使用DMA传输**
   - 减少CPU占用
   - 提高传输效率

2. **多块传输**
   - 减少命令开销
   - 提高连续传输速度

3. **高速模式**
   - HS200: 200MHz
   - HS400: 400MHz

4. **8位总线**
   - 最大数据带宽

### 7.2 性能指标

| 模式 | 时钟频率 | 总线宽度 | 理论带宽 |
|-----|---------|---------|---------|
| Legacy | 26MHz | 1-bit | 3.25 MB/s |
| High Speed | 52MHz | 4-bit | 26 MB/s |
| High Speed | 52MHz | 8-bit | 52 MB/s |
| HS200 | 200MHz | 8-bit | 200 MB/s |
| HS400 | 400MHz | 8-bit | 400 MB/s |

## 8. 移植注意事项

### 8.1 平台相关部分

需要根据实际硬件修改：

1. 寄存器基地址
2. 时钟配置
3. 延时函数实现
4. 中断配置
5. DMA配置

### 8.2 移植检查清单

- [ ] 修改寄存器基地址
- [ ] 实现延时函数
- [ ] 配置中断向量
- [ ] 测试时钟配置
- [ ] 验证DMA功能
- [ ] 测试所有API
- [ ] 压力测试
- [ ] 功耗优化

## 9. 调试建议

### 9.1 调试工具

- 逻辑分析仪：观察CMD/DAT信号
- 示波器：检查时序和信号质量
- 串口日志：记录操作流程
- 寄存器dump：查看寄存器状态

### 9.2 常见问题

1. **初始化失败**
   - 检查电源和时钟
   - 验证复位时序
   - 查看卡是否存在

2. **读写错误**
   - 检查地址范围
   - 验证数据线连接
   - 检查CRC错误

3. **超时错误**
   - 检查时钟配置
   - 验证命令参数
   - 查看卡状态

## 10. 未来扩展

### 10.1 计划功能

- [ ] HS400模式支持
- [ ] eMMC 5.1高级特性
- [ ] 电源管理
- [ ] 可靠写入
- [ ] 分区管理
- [ ] 安全特性

### 10.2 优化方向

- [ ] 性能优化
- [ ] 代码重构
- [ ] 文档完善
- [ ] 测试覆盖
- [ ] 错误恢复增强

---

**文档版本：** 1.0  
**最后更新：** 2025-11-13
