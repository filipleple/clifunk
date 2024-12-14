# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I./src
LDFLAGS = -lncurses -lpulse-simple -lpulse -lm

# Directories
SRC_DIR = src
BUILD_DIR = build

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = $(BUILD_DIR)/render_engine

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
