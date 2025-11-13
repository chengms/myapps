# eMMC 寄存器模拟说明

本文件描述 `sim/emmc_hw_sim.c` 中实现的寄存器集合及其行为，便于与真实芯片手册对照使用。

## 寄存器概览

| 偏移地址 | 名称              | 描述 |
|----------|-------------------|------|
| 0x00     | SYSTEM_CTRL       | 主控系统控制寄存器，当前用于配置时钟、启动标志等（模拟中保留）。 |
| 0x04     | CMD               | 命令寄存器，写入命令索引后立即触发执行，并在完成时设置 `CMD_COMPLETE` 中断。 |
| 0x08     | ARG               | 命令参数寄存器，对应 eMMC 命令的 Argument 字段。 |
| 0x0C-0x18| RESP0-RESP3       | 命令响应寄存器，目前用于保留未来扩展。 |
| 0x1C     | DATA              | 数据端口，中间缓存通道（当前模拟未直接使用）。 |
| 0x20     | STATUS            | 状态寄存器，包含 `READY`、`CMD_INHIBIT`、`DATA_INHIBIT`、`ERROR` 标志。 |
| 0x24     | INT_STATUS        | 中断状态寄存器，通过写 1 清除对应位。 |
| 0x28     | INT_ENABLE        | 中断使能寄存器，默认开启命令、数据完成与错误中断。 |
| 0x2C     | BLOCK_SIZE        | 块大小寄存器，默认 512 字节。 |
| 0x30     | BLOCK_COUNT       | 块计数寄存器，记录最近一次传输块数。 |
| 0x34     | CLOCK_CTRL        | 时钟控制寄存器，保存固件配置的主频值。 |
| 0x38     | RESET             | 复位寄存器，写入 `RESET_ALL` 时会复位整个模拟硬件。 |

## 标志位定义

- `STATUS.READY`：主控空闲，可接受新的命令与数据请求。
- `STATUS.CMD_INHIBIT`：命令执行中，自复位。
- `STATUS.DATA_INHIBIT`：数据传输中，自复位。
- `STATUS.ERROR`：命令或数据越界等错误，需由固件清除。
- `INT_STATUS.CMD_COMPLETE`：命令完成中断。
- `INT_STATUS.DATA_COMPLETE`：数据传输完成中断。
- `INT_STATUS.ERROR`：错误中断。

## 存储阵列

- 块大小：固定为 512 字节。
- 默认容量：4096 块（约 2 MiB），可在 `sim/emmc_hw_sim.c` 中通过 `EMMC_SIM_BLOCKS` 调整。
- 访问检查：读写时若 `LBA + 块数` 超出容量，将置位 `STATUS.ERROR` 并触发 `INT_STATUS.ERROR`。

## 扩展钩子

为了更好地映射真实硬件，可在以下位置扩展逻辑：

- `emmc_hw_write_reg()`：根据命令索引填充响应寄存器，或模拟耗时。
- `emmc_hw_read_blocks()/emmc_hw_write_blocks()`：加入 CRC、延时、带宽限制或坏块注入。
- `emmc_hw_init()`：读取外部配置文件，动态模拟不同容量、总线宽度及厂商特性。
