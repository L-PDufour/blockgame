#include "../inc/game.h"
#include "raylib.h"
#include "stdint.h"
#include <stdio.h>

void drawTile(s_GameState *state, int x, int y, int tileType) {
  DrawTexturePro(
      *state->assets->biomeTextures, state->assets->biomeRects[tileType],
      (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE},
      (Vector2){0, 0}, 0.0f, WHITE);
}

int determineTileType(int x, int y) {
  s_GameState *state = getGameState();
  e_BiomeTypes biome = state->worldMap->tiles[y][x].type;

  switch (biome) {
  case BIOME_WATER:
    return BIOME_WATER;
  case BIOME_BEACH:
    return BIOME_BEACH;
  case BIOME_PLAINS:
    return BIOME_PLAINS;
  case BIOME_FOREST:
    return BIOME_FOREST;
  case BIOME_HILL:
    return BIOME_HILL;
  case BIOME_MOUNTAIN:
    return BIOME_MOUNTAIN;
  default:
    return BIOME_PLAINS; // Default fallback
  }
}

void drawWorldMap() {
  s_GameState *state = getGameState();
  BeginTextureMode(state->worldMap->mapTexture);
  ClearBackground(RAYWHITE);

  int map_width = SCREEN_WIDTH / TILE_SIZE;
  int map_height = SCREEN_HEIGHT / TILE_SIZE;

  for (int y = 0; y < map_height; y++) {
    for (int x = 0; x < map_width; x++) {

      int tileType = determineTileType(x, y);
      drawTile(state, x, y, tileType);
    }
  }
  EndTextureMode();
}
