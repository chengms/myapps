CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -Iinclude -Isim
LDFLAGS ?=

BUILD_DIR := build
TARGET := $(BUILD_DIR)/emmc_sim
TEST_TARGET := $(BUILD_DIR)/emmc_tests

CORE_SRC := $(filter-out src/main.c,$(wildcard src/*.c))
SIM_SRC := $(wildcard sim/*.c)
APP_SRC := src/main.c

CORE_OBJ := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(CORE_SRC))
SIM_OBJ := $(patsubst sim/%.c,$(BUILD_DIR)/%.o,$(SIM_SRC))
APP_OBJ := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(APP_SRC))
COMMON_OBJ := $(CORE_OBJ) $(SIM_OBJ)

TEST_SRC := tests/test_emmc.c
TEST_OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(TEST_SRC)))

all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: sim/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(BUILD_DIR) $(COMMON_OBJ) $(APP_OBJ)
	$(CC) $(CFLAGS) $(COMMON_OBJ) $(APP_OBJ) $(LDFLAGS) -o $@

$(TEST_TARGET): $(BUILD_DIR) $(COMMON_OBJ) $(BUILD_DIR)/$(notdir $(TEST_OBJ))
	$(CC) $(CFLAGS) $(COMMON_OBJ) $(BUILD_DIR)/$(notdir $(TEST_OBJ)) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(notdir $(TEST_OBJ)): $(TEST_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean run test

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)
