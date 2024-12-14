# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -lncurses -I./src

# Directories
SRC_DIR = src
BUILD_DIR = build

# Files and Targets
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/canvas.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/canvas.o
TARGET = $(BUILD_DIR)/render_engine

# Build Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
