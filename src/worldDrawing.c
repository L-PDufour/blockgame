#include "../inc/game.h"

void drawTile(s_GameState *state, int x, int y, int tileType) {
  DrawTexturePro(
      *state->assets->grassTextures, state->assets->grassRects[tileType],
      (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE},
      (Vector2){0, 0}, 0.0f, WHITE);
}

int determineTileType(int x, int y, int map_width, int map_height) {
  // Your existing tile type determination logic
  if (x == 0 && y == 0)
    return NE_GRASS;
  else if (x == map_width - 1 && y == 0)
    return NW_GRASS; // Top-right corner
  else if (x == 0 && y == map_height - 1)
    return SE_GRASS; // Bottom-left corner
  else if (x == map_width - 1 && y == map_height - 1)
    return SW_GRASS; // Bottom-right corner
  else if (x == 0)
    return E_GRASS; // Left edge
  else if (x == map_width - 1)
    return W_GRASS; // Right edge
  else if (y == 0)
    return N_GRASS; // Top edge
  else if (y == map_height - 1)
    return S_GRASS; // Bottom edge
  return M_GRASS;
}

void drawWorldMap() {
  s_GameState *state = getGameState();
  int map_width = SCREEN_WIDTH / TILE_SIZE;
  int map_height = SCREEN_HEIGHT / TILE_SIZE;

  for (int i = 0; i < 12; i++) {
    int x = (i % 4) * TILE_SIZE;
    int y = (i / 4) * TILE_SIZE;
    state->assets->grassRects[i] = (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
  }
  for (int i = 0; i < 12; i++) {
    int x = (i % 2) * TILE_SIZE;
    int y = (i / 2) * TILE_SIZE;
    state->assets->biomeRects[i] = (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
  }

  for (int y = 0; y < map_height; y++) {
    for (int x = 0; x < map_width; x++) {

      int tileType = determineTileType(x, y, map_width, map_height);
      drawTile(state, x, y, tileType);
    }
  }
  DrawTexturePro(
      *state->assets->biomeTextures, state->assets->biomeRects[1],
      (Rectangle){1 * TILE_SIZE, 1 * TILE_SIZE, TILE_SIZE, TILE_SIZE},
      (Vector2){0, 0}, 0.0f, WHITE);
}
