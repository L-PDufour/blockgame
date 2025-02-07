#include "../inc/game.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

s_GameState *state = NULL;

s_GameState *getGameState() {
  if (state == NULL) {
    return NULL;
  }

  return state;
}

void drawCharacter(e_CharacterType character, Vector2 position) {
  s_GameState *state = getGameState();

  if (state == NULL)
    return; // Ensure state is not NULL

  // Select the current texture based on the current frame
  Texture2D currentTexture = (state->currentFrame == 0)
                                 ? state->assets->characterFrames[0]
                                 : state->assets->characterFrames[1];

  // Draw the character using DrawTexturePro
  DrawTexturePro(currentTexture, state->assets->characterRects[character],
                 (Rectangle){position.x, position.y,
                             TILE_SIZE, // Scale the texture
                             TILE_SIZE},
                 (Vector2){0, 0}, // Origin point for rotation
                 0.0f,            // No rotation
                 WHITE            // Color tint
  );
}

void drawScene() {
  s_GameState *state = getGameState();

  if (state == NULL)
    return; // Ensure state is not NULL
  // Draw the character at the hero's position
  drawCharacter(CHARACTER_1, (Vector2){.x = state->hero->entityDest.x,
                                       .y = state->hero->entityDest.y});
}

void updatePlayerAnimation() {
  s_GameState *state = getGameState();

  if (state->hero->isMoving) { // Only animate when moving
    state->frameTime += GetFrameTime();

    if (state->frameTime >= 1.0f / 8.0f) { // 12 FPS animation
      state->currentFrame =
          (state->currentFrame + 1) % 2; // Toggle between 0 and 1
      state->frameTime = 0;              // Reset frame time
    }
  } else {
    state->currentFrame = 0;
  }
}

void update() {
  s_GameState *state = getGameState();
  float smoothSpeed = 0.5f; // Adjust this value to change smoothing (0.1 = more
                            // smooth, 1.0 = no smoothing)

  // Calculate target position (center on player)
  Vector2 targetPos = {
      state->hero->entityDest.x + (state->hero->entityDest.width / 2),
      state->hero->entityDest.y + (state->hero->entityDest.height / 2)};

  // Update camera position smoothly
  state->camera.target.x +=
      (targetPos.x - state->camera.target.x) * smoothSpeed;
  state->camera.target.y +=
      (targetPos.y - state->camera.target.y) * smoothSpeed;
}

void render() {
  s_GameState *state = getGameState();
  BeginDrawing();
  BeginMode2D(state->camera); // You were missing BeginMode2D
  ClearBackground(RAYWHITE);
      DrawTextureRec(state->worldMap->mapTexture.texture,
                   (Rectangle){ 0, 0,
                                (float)state->worldMap->mapTexture.texture.width,
                                -(float)state->worldMap->mapTexture.texture.height },
                   (Vector2){ 0, 0 }, WHITE);
  drawScene();
  EndMode2D();
  EndDrawing();
}

void loadAssets() {
  s_GameState *state = getGameState();

  state->assets->characterFrames[0] = LoadTexture("ressources/human0.png");
  if (state->assets->characterFrames[0].id == 0) {
    printf("Failed to load human0.png\n");
    return;
  }

  state->assets->characterFrames[1] = LoadTexture("ressources/human1.png");
  if (state->assets->characterFrames[1].id == 0) {
    printf("Failed to load human1.png\n");
    UnloadTexture(state->assets->characterFrames[0]); // Clean up
    return;
  }

  for (int i = 0; i < 9; i++) {
    int x = (i % 3) * TILE_SIZE;
    int y = (i / 3) * TILE_SIZE;
    state->assets->characterRects[i] = (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
  }
  for (int i = 0; i < 6; i++) {
    int x = i * TILE_SIZE; // Just multiply by i since it's a single row
    int y = 0;             // y stays 0 since we're only using one row
    state->assets->biomeRects[i] = (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
  }
  *state->assets->grassTextures = LoadTexture("ressources/grass.png");
  *state->assets->biomeTextures = LoadTexture("ressources/biomes.png");
}

s_WorldMap *createWorldMap() {
  s_WorldMap *worldMap = malloc(sizeof(s_WorldMap));
  worldMap->width = SCREEN_WIDTH / TILE_SIZE;
  worldMap->height = SCREEN_HEIGHT / TILE_SIZE;

  // Allocate memory for tiles
  worldMap->tiles = malloc(sizeof(s_WorldTile *) * worldMap->height);
  for (int y = 0; y < worldMap->height; y++) {
    worldMap->tiles[y] = malloc(sizeof(s_WorldTile) * worldMap->width);
  }

  // Generate noise map
  int randomOffsetX = GetRandomValue(0, 10000);
  int randomOffsetY = GetRandomValue(0, 10000);
  Image noiseMap = GenImagePerlinNoise(worldMap->width, worldMap->height,
                                       randomOffsetX, randomOffsetY, 4.0f);

  for (int y = 0; y < worldMap->height; y++) {
    for (int x = 0; x < worldMap->width; x++) {
      Color pixel = GetImageColor(noiseMap, x, y);
      float value = pixel.r / 255.0f; // Convert to 0-1 range

      if (value < 0.2f) {
        worldMap->tiles[y][x].type = BIOME_WATER;
      } else if (value < 0.35f) {
        worldMap->tiles[y][x].type = BIOME_BEACH;
      } else if (value < 0.5f) {
        worldMap->tiles[y][x].type = BIOME_PLAINS;
      } else if (value < 0.65f) {
        worldMap->tiles[y][x].type = BIOME_FOREST;
      } else if (value < 0.85f) {
        worldMap->tiles[y][x].type = BIOME_HILL;
      } else {
        worldMap->tiles[y][x].type = BIOME_MOUNTAIN;
      }
    }
  }

  UnloadImage(noiseMap);
  return worldMap;
}

void destroyWorldMap(s_WorldMap *worldMap) {
  for (int y = 0; y < worldMap->height; y++) {
    free(worldMap->tiles[y]);
  }
  free(worldMap->tiles);
  free(worldMap);
}

s_GameState *initGameState() {
  if (state == NULL) {
    state = malloc(sizeof(s_GameState));
  }

  state->assets = malloc(sizeof(s_AssetManager));
  state->assets->grassTextures = malloc(sizeof(Texture2D));
  state->assets->biomeTextures = malloc(sizeof(Texture2D));

  state->worldMap = createWorldMap();
  state->worldMap->mapTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

  if (state->worldMap == NULL) {
    free(state->assets);
    free(state);
    return NULL;
  }

  loadAssets();
  if (state->assets->characterFrames[0].id == 0 ||
      state->assets->characterFrames[1].id == 0) {
    destroyWorldMap(state->worldMap);
    free(state->assets);
    free(state);
    return NULL;
  }

  state->hero = malloc(sizeof(s_Entity));
  state->hero->entityDest = (Rectangle){200, 200, TILE_SIZE, TILE_SIZE};
  state->camera.target =
      (Vector2){state->hero->entityDest.x + state->hero->entityDest.width / 2,
                state->hero->entityDest.y + state->hero->entityDest.height / 2};
  state->camera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  state->camera.rotation = 0.0f;
  state->camera.zoom = 3.0f;
  state->currentFrame = 0;
  state->frameTime = 0;

  drawWorldMap();
  return state;
}
void cleanupGame() {
  s_GameState *state = getGameState();
  UnloadRenderTexture(state->worldMap->mapTexture);
  // ... other cleanup code ...
}
void quit() {
  s_GameState *state = getGameState();
  if (state) {
    if (state->assets) {
      UnloadTexture(state->assets->characterFrames[0]);
      UnloadTexture(state->assets->characterFrames[1]);
      UnloadTexture(*state->assets->biomeTextures);
      UnloadTexture(*state->assets->grassTextures);
      free(state->assets);
    }
    if (state->hero) {
      free(state->hero);
    }
    destroyWorldMap(state->worldMap);
    free(state);
  }
  cleanupGame();
  CloseWindow();
}

int main(void) {
  printf("Initializing game state...\n");
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Game");
  state = initGameState();

  if (state == NULL) {
    printf("Failed to initialize game state\n");
    return 1;
  }

  printf("Initializing window and setting FPS...\n");

  SetTargetFPS(60);

  printf("Entering game loop...\n");
  while (!WindowShouldClose()) {

    input();
    updatePlayerAnimation();
    update();
    render();
  }

  printf("Quitting game...\n");
  quit();
  return 0;
}
