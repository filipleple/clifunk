# Variables
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I./src/include -I./src/lib/kissfft
LDFLAGS = -lncurses -lpulse -lpulse-simple -lm
BUILD_DIR = build
SRC_DIR = src
KISSFFT_DIR = src/lib/kissfft

# Source files and corresponding object files
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(KISSFFT_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
OBJECTS += $(patsubst $(KISSFFT_DIR)/%.c, $(BUILD_DIR)/lib/kissfft/%.o, $(wildcard $(KISSFFT_DIR)/*.c))

# Target executable
TARGET = $(BUILD_DIR)/render_engine

# Default rule
all: $(TARGET)

# Linking step (note order: OBJECTS first, LDFLAGS last)
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compilation step for source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Compilation step for KISS FFT library files
$(BUILD_DIR)/lib/kissfft/%.o: $(KISSFFT_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean rule to remove build files
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/lib $(TARGET)

