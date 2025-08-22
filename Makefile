# Use the following if you're using the nix-shell for development
# Set Emscripten cache to a writable location
export EM_CACHE = $(HOME)/.cache/emscripten
SHELL := /usr/bin/env bash
RAYLIB ?= ./dep/lib
RAYLIB_WEB ?= ./dep/lib/web
INC_DIR = inc
SRC_DIR = src
CC = gcc
EMCC = emcc
CFLAGS = -std=c99 -Wall -Wextra -I$(RAYLIB) -I$(INC_DIR)
LIBS = -lraylib -lGL -lm -lpthread -ldl -lX11 -lwayland-client -lwayland-cursor

# Web configuration
WEB_DIR = web
WEB_OUT = $(WEB_DIR)/index.html
EMCC_FLAGS = -std=c99 -Wall -Wextra -I$(RAYLIB_WEB) -I$(RAYLIB) -I$(INC_DIR) -DPLATFORM_WEB -O2
EMCC_LIBS = $(RAYLIB_WEB)/libraylib.a
EMCC_LINK_FLAGS = -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=33554432 -s ALLOW_MEMORY_GROWTH=0 -s FORCE_FILESYSTEM=1 -s ASSERTIONS=0 -s WASM=1 -s LEGACY_GL_EMULATION=1 -s GL_UNSAFE_OPTS=0 -s MIN_WEBGL_VERSION=1 -s MAX_WEBGL_VERSION=1 --shell-file $(RAYLIB_WEB)/shell.html --preload-file assets --minify 0

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/input.c $(SRC_DIR)/world_map.c $(SRC_DIR)/assets.c $(SRC_DIR)/character.c $(SRC_DIR)/game_state.c $(SRC_DIR)/entity.c

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

# Debug version for troubleshooting iOS issues
web-debug: create-web-dir
	$(EMCC) $(EMCC_FLAGS) $(SRCS) $(EMCC_LIBS) -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=33554432 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s DEMANGLE_SUPPORT=1 --shell-file $(RAYLIB_WEB)/shell.html --preload-file assets -o $(WEB_OUT)

# Legacy version for older iOS (asm.js fallback)
web-legacy: create-web-dir
	$(EMCC) $(EMCC_FLAGS) $(SRCS) $(EMCC_LIBS) -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=16777216 -s WASM=0 -s LEGACY_GL_EMULATION=1 --preload-file assets -o $(WEB_DIR)/index.js
	@echo '<!DOCTYPE html><html><head><meta charset="utf-8"><title>RayWasm Game (Legacy)</title><style>body{margin:0;padding:0;background:#000;display:flex;justify-content:center;align-items:center;min-height:100vh;}canvas{border:1px solid #333;}</style></head><body><canvas id="canvas" oncontextmenu="event.preventDefault()"></canvas><script>var Module={canvas:document.getElementById("canvas"),printErr:console.error,setStatus:function(text){console.log("Status: "+text)}};</script><script src="index.js"></script></body></html>' > $(WEB_DIR)/index.html

# Simple web build without HTML shell template (avoids npm dependency)
web-simple: create-web-dir
	$(EMCC) $(EMCC_FLAGS) $(SRCS) $(EMCC_LIBS) -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=33554432 -s ALLOW_MEMORY_GROWTH=0 -s FORCE_FILESYSTEM=1 -s ASSERTIONS=0 -s WASM=1 -s LEGACY_GL_EMULATION=1 -s GL_UNSAFE_OPTS=0 -s MIN_WEBGL_VERSION=1 -s MAX_WEBGL_VERSION=1 --preload-file assets -o $(WEB_DIR)/index.js
	@echo '<!DOCTYPE html><html><head><meta charset="utf-8"><title>RayWasm Game</title><style>body{margin:0;padding:0;background:#000;display:flex;justify-content:center;align-items:center;min-height:100vh;}canvas{border:1px solid #333;}</style></head><body><canvas id="canvas" oncontextmenu="event.preventDefault()"></canvas><script>var Module={canvas:document.getElementById("canvas"),printErr:console.error,setStatus:function(text){console.log("Status: "+text)}};</script><script src="index.js"></script></body></html>' > $(WEB_DIR)/index.html

run-web-simple: web-simple
	python3 -m http.server --directory $(WEB_DIR) 8080 --bind 0.0.0.0

run-web-legacy: web-legacy
	python3 -m http.server --directory $(WEB_DIR) 8080 --bind 0.0.0.0

# Ultra-compatible version for very old iOS
web-ios12: create-web-dir
	$(EMCC) $(EMCC_FLAGS) $(SRCS) $(EMCC_LIBS) \
	-s USE_GLFW=3 \
	-s ASYNCIFY \
	-s TOTAL_MEMORY=16777216 \
	-s ALLOW_MEMORY_GROWTH=0 \
	-s WASM=0 \
	-s LEGACY_GL_EMULATION=1 \
	-s USE_WEBGL2=0 \
	-s MIN_WEBGL_VERSION=1 \
	-s MAX_WEBGL_VERSION=1 \
	-s GL_UNSAFE_OPTS=0 \
	-s ASSERTIONS=0 \
	--preload-file assets \
	-o $(WEB_DIR)/index.js
	@echo '<!DOCTYPE html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>RayWasm Game</title><style>body{margin:0;padding:20px;background:#222;font-family:Arial,sans-serif;color:white;}#gameContainer{text-align:center;}canvas{border:1px solid #555;max-width:100%;height:auto;}#status{margin:10px 0;}</style></head><body><div id="gameContainer"><h3>RayWasm Game</h3><div id="status">Loading...</div><canvas id="canvas" oncontextmenu="event.preventDefault()"></canvas></div><script>var Module={canvas:document.getElementById("canvas"),printErr:function(text){console.error(text);document.getElementById("status").innerHTML="Error: "+text;},print:function(text){console.log(text);},setStatus:function(text){console.log("Status: "+text);document.getElementById("status").innerHTML=text;if(!text)document.getElementById("status").style.display="none";},totalDependencies:0,monitorRunDependencies:function(left){this.totalDependencies=Math.max(this.totalDependencies,left);Module.setStatus(left?"Preparing... ("+(this.totalDependencies-left)+"/"+this.totalDependencies+")":"All downloads complete.");}};</script><script src="index.js"></script></body></html>' > $(WEB_DIR)/index.html

# Even simpler version - minimal flags
web-minimal: create-web-dir
	$(EMCC) $(EMCC_FLAGS) $(SRCS) $(EMCC_LIBS) \
	-s USE_GLFW=3 \
	-s ASYNCIFY \
	-s TOTAL_MEMORY=16777216 \
	-s WASM=0 \
	--preload-file assets \
	-o $(WEB_DIR)/index.js
	@echo '<!DOCTYPE html><html><head><meta charset="utf-8"><title>Game</title></head><body><canvas id="canvas"></canvas><script>var Module={canvas:document.getElementById("canvas")};</script><script src="index.js"></script></body></html>' > $(WEB_DIR)/index.html

# Version based on working iPad script
run-web-ipad-simple:
	mkdir -p web
	emcc -std=c++17 -Wall -O2 \
	src/main.c src/input.c src/world_map.c src/assets.c src/character.c src/game_state.c src/entity.c \
	./dep/lib/web/libraylib.a \
	-s USE_GLFW=3 \
	-s ASYNCIFY \
	-s TOTAL_MEMORY=67108864 \
	-s FORCE_FILESYSTEM=1 \
	-s EXPORTED_FUNCTIONS=_main \
	--preload-file assets \
	-o web/index.html

run-web-ipad: web-ipad
	python3 -m http.server --directory $(WEB_DIR) 8080 --bind 0.0.0.0

run-web-minimal: web-minimal
	python3 -m http.server --directory $(WEB_DIR) 8080 --bind 0.0.0.0

.PHONY: clean rebuild rebuild-web run run-web web create-web-dir check-syntax web-debug web-legacy web-simple run-web-simple run-web-legacy web-ios12 run-web-ios12 web-minimal run-web-minimal web-ipad run-web-ipad

run-web-ios12: web-ios12
	python3 -m http.server --directory $(WEB_DIR) 8080 --bind 0.0.0.0

.PHONY: clean rebuild rebuild-web run run-web web create-web-dir check-syntax web-debug web-legacy web-simple run-web-simple run-web-legacy web-ios12 run-web-ios12
