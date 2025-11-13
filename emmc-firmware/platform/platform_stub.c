#include "hal_emmc.h"

// 预留平台相关的钩子函数，当前仿真环境下为空实现。
void platform_board_init(void) {
  hal_emmc_log("[PLATFORM] board init stub");
}

void platform_board_deinit(void) {
  hal_emmc_log("[PLATFORM] board deinit stub");
}
