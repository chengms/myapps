CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -g
INCLUDES := -Ifirmware/include -Ihal/include -Isim -Iplatform -Itests
LDFLAGS :=

BUILD_DIR := build
TARGET := $(BUILD_DIR)/emmc_sim

SRC_COMMON := $(wildcard firmware/src/*.c) \
               $(wildcard hal/src/*.c) \
               $(wildcard sim/*.c) \
               $(wildcard platform/*.c)

TEST_SRC := tests/emmc_smoke_test.c

OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_COMMON) $(TEST_SRC))

.PHONY: all clean test dirs

all: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

# 运行默认测试（当前为冒烟测试可执行文件）
test: all
	./$(TARGET)

# 方便在无源文件时预创建 build 目录
dirs:
	@mkdir -p $(BUILD_DIR)
