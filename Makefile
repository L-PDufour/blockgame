RAYLIB ?= ./external/raylib-5.5/src/
INC_DIR = inc
SRC_DIR = src
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I$(RAYLIB) -I$(INC_DIR)
LIBS = -L$(RAYLIB) -lraylib -lm -lpthread -ldl -lGL -lX11 -lwayland-client -lwayland-cursor

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/input.c $(SRC_DIR)/worldDrawing.c

# Output binary
OUT = game

# Compile and link
$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(OUT)

# Clean up generated files
clean:
	rm -f $(OUT)

# Rebuild the project
rebuild: clean $(OUT)

run: rebuild
	./$(OUT)

check-syntax:
	$(CC) $(CFLAGS) -Wall -Wextra -Wpedantic -fsyntax-only $(SRCS)

.PHONY: clean rebuild run check-syntax
