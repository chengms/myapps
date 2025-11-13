cmake_minimum_required(VERSION 3.10)
project(emmc_firmware C)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2 -g")

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    src/emmc_driver.c
    src/hal.c
    src/main.c
)

# 创建可执行文件
add_executable(emmc_firmware ${SOURCES})

# 创建静态库（不包含main.c）
set(LIB_SOURCES
    src/emmc_driver.c
    src/hal.c
)

add_library(emmc_static STATIC ${LIB_SOURCES})

# 安装规则
install(TARGETS emmc_firmware emmc_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
