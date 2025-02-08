# Use the following if you're using the nix-shell for development
# Set Emscripten cache to a writable location
export EM_CACHE = $(HOME)/.cache/emscripten
SHELL := /usr/bin/env bash

RAYLIB ?= ./dep/lib
RAYLIB_WEB ?= .dep/lib/web
INC_DIR = inc
SRC_DIR = src
CC = gcc
EMCC = emcc
CFLAGS = -std=c99 -Wall -Wextra -I$(RAYLIB) -I$(INC_DIR)
LIBS = -lraylib -lGL -lm -lpthread -ldl -lX11 -lwayland-client -lwayland-cursor

# Web configuration

WEB_DIR = web
WEB_OUT = $(WEB_DIR)/index.html
EMCC_FLAGS = -std=c99 -Wall -Wextra -I$(RAYLIB_WEB) -I$(RAYLIB) -I$(INC_DIR) -DPLATFORM_WEB
EMCC_LIBS = $(RAYLIB_WEB)/libraylib.a
EMCC_LINK_FLAGS = -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 \
                  -s FORCE_FILESYSTEM=1 -s ASSERTIONS=1 \
                  --shell-file $(RAYLIB_WEB)/shell.html \
                  --preload-file assets

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/input.c $(SRC_DIR)/world_map.c \
       $(SRC_DIR)/assets.c $(SRC_DIR)/character.c $(SRC_DIR)/game_state.c \
       $(SRC_DIR)/entity.c

# Output binary
OUT = game

# Native build
$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(OUT)

# Web build
web: create-web-dir $(WEB_OUT)

create-web-dir:
	mkdir -p $(WEB_DIR)

$(WEB_OUT): $(SRCS)
	$(EMCC) $(EMCC_FLAGS) $(SRCS) $(EMCC_LIBS) $(EMCC_LINK_FLAGS) -o $(WEB_OUT)

# Clean up generated files
clean:
	rm -f $(OUT)
	rm -rf $(WEB_DIR)

# Rebuild projects
rebuild: clean $(OUT)

rebuild-web: clean web

# Run targets
run: rebuild
	./$(OUT)

run-web: rebuild-web
	python3 -m http.server --directory $(WEB_DIR) 8080

check-syntax:
	$(CC) $(CFLAGS) -Wall -Wextra -Wpedantic -fsyntax-only $(SRCS)

.PHONY: clean rebuild rebuild-web run run-web web create-web-dir check-syntax
