# ====================================
# eMMC固件开发工程 Makefile
# ====================================

# 编译器配置
CC := gcc
AR := ar
OBJCOPY := objcopy

# 项目名称
PROJECT := emmc_firmware
TARGET := $(PROJECT).elf
TEST_TARGET := test_emmc

# 目录配置
SRC_DIR := src
INC_DIR := inc
TEST_DIR := test
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

# 源文件
SRCS := $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

# 目标文件
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SRCS))

# 编译标志
CFLAGS := -Wall -Wextra -std=c99 -O2
CFLAGS += -I$(INC_DIR)

# 调试标志
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -g -DDEBUG
endif

# 链接标志
LDFLAGS := 

# 库
LIBS := 

# ====================================
# 编译规则
# ====================================

.PHONY: all clean test lib help dirs

# 默认目标
all: dirs $(BIN_DIR)/lib$(PROJECT).a

# 创建必要的目录
dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# 编译源文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "编译: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# 编译测试文件
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	@echo "编译测试: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# 生成静态库
$(BIN_DIR)/lib$(PROJECT).a: $(OBJS)
	@echo "生成静态库: $@"
	@$(AR) rcs $@ $^
	@echo "✓ 静态库生成完成"

# 编译测试程序
test: dirs $(BIN_DIR)/$(TEST_TARGET)

$(BIN_DIR)/$(TEST_TARGET): $(TEST_OBJS) $(OBJS)
	@echo "链接测试程序: $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo "✓ 测试程序编译完成"

# 运行测试
run-test: test
	@echo ""
	@echo "========================================="
	@echo " 运行eMMC驱动测试程序"
	@echo "========================================="
	@$(BIN_DIR)/$(TEST_TARGET)

# 清理
clean:
	@echo "清理编译文件..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ 清理完成"

# 显示帮助
help:
	@echo ""
	@echo "eMMC固件开发工程 - Makefile使用说明"
	@echo "===================================="
	@echo "make          - 编译生成静态库"
	@echo "make test     - 编译测试程序"
	@echo "make run-test - 编译并运行测试程序"
	@echo "make clean    - 清理编译文件"
	@echo "make help     - 显示此帮助信息"
	@echo ""
	@echo "调试模式: make DEBUG=1"
	@echo ""

# 显示编译信息
info:
	@echo ""
	@echo "编译配置信息:"
	@echo "  编译器: $(CC)"
	@echo "  编译标志: $(CFLAGS)"
	@echo "  链接标志: $(LDFLAGS)"
	@echo "  源文件: $(SRCS)"
	@echo "  目标文件: $(OBJS)"
	@echo ""

# 代码统计
stats:
	@echo ""
	@echo "代码统计:"
	@echo "========================================="
	@wc -l $(INC_DIR)/*.h $(SRC_DIR)/*.c $(TEST_DIR)/*.c
	@echo "========================================="
	@echo ""
