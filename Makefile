# Basic SDL3 Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LIBS = `pkg-config --libs sdl3`
INCLUDES = `pkg-config --cflags sdl3`

# Directories
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game

# Source files
SOURCES = $(SRC_DIR)/main.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build executable
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LIBS) -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run the game
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Hot reload development
watch: $(TARGET)
	@echo "🔥 Hot reload enabled - edit src/ files to rebuild"
	@find $(SRC_DIR) -name "*.c" -o -name "*.h" | entr -r make run

# Install file watcher if not available
install-entr:
	@if ! command -v entr >/dev/null 2>&1; then \
		echo "Installing entr for hot reload..."; \
		nix-env -iA nixpkgs.entr; \
	fi

# Phony targets
.PHONY: all run clean debug watch install-entr
