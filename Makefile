# Variables
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I./src/include
LDFLAGS = -lncurses -lpulse -lpulse-simple -lm 
BUILD_DIR = build
SRC_DIR = src

# Source files and corresponding object files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Target executable
TARGET = $(BUILD_DIR)/render_engine

# Default rule
all: $(TARGET)

# Linking step (note order: OBJECTS first, LDFLAGS last)
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compilation step for each source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean rule to remove build files
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

