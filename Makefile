
RAYLIB ?= ./external/raylib-5.5/src/

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I$(RAYLIB)
LIBS = -L$(RAYLIB) -lraylib -lm -lpthread -ldl -lGL -lX11 -lwayland-client -lwayland-cursor

# Source files
SRCS = main.c

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

