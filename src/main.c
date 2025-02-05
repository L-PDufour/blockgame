#include "../inc/game.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

s_GameState *state = NULL;

s_GameState *getGameState() {
  if (state == NULL) {
    state = malloc(sizeof(s_GameState));
    if (state == NULL) {
      printf("Failed to allocate memory for game state\n");
      return NULL;
    }
    state->assets = malloc(sizeof(s_AssetManager));
    if (state->assets == NULL) {
      free(state);
      printf("Failed to allocate memory for asset manager\n");
      return NULL;
    }
    // Initialize other fields as necessary
    state->hero = malloc(sizeof(s_Entity));
    if (state->hero == NULL) {
      free(state->assets);
      free(state);
      printf("Failed to allocate memory for hero\n");
      return NULL;
    }
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
  drawWorldMap();
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
  *state->assets->grassTextures = LoadTexture("ressources/grass.png");
  *state->assets->biomeTextures = LoadTexture("ressources/biome.png");
}

void generateBiome(s_WorldMap *map, e_BiomeTypes type) {

  for (int y = 0; y < map->height; y++) {
    for (int x = 0; x < map->width; x++) {

    }
  }
}

void generate_patch(s_WorldMap* map, e_BiomeTypes biome_type, int num_patches, int min_size, int max_size, int irregular) {
    srand(time(NULL)); // Seed random number generator
                printf("ici\n");
    for (int p = 0; p < num_patches; p++) {
        int width = rand() % (max_size - min_size + 1) + min_size;
        int height = rand() % (max_size - min_size + 1) + min_size;
        int start_x = rand() % (map->width - width);
        int start_y = rand() % (map->height - height);

        if (irregular) {
            int init_start_x = rand() % (map->width - max_size + 1);
            for (int i = 0; i < height; i++) {
                width = rand() % (max_size - (int)(0.7 * max_size) + 1) + (int)(0.7 * max_size);
                start_x = init_start_x - (rand() % 3); // Randomly adjust start_x by -1 or -2
                for (int j = 0; j < width; j++) {
                }
                printf("ici\n");
                init_start_x = start_x; // Update init_start_x for the next row
            }
        } else {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                }
            }
        }
    }
}
// World map initialization
s_WorldMap *createWorldMap() {
  s_WorldMap *worldMap = malloc(sizeof(s_WorldMap));
  worldMap->width = SCREEN_WIDTH / TILE_SIZE;
  worldMap->height = SCREEN_HEIGHT / TILE_SIZE;
            printf("ici\n");
  // Allocate 2D array of tiles
  worldMap->tiles = malloc(sizeof(s_WorldTile *) * worldMap->height);
  for (int y = 0; y < worldMap->height; y++) {
    worldMap->tiles[y] = malloc(sizeof(s_WorldTile) * worldMap->width);
  }

  for (int y = 0; y < worldMap->height; y++) {
    for (int x = 0; x < worldMap->width; x++) {
      worldMap->tiles[y][x].type = GRASSLANDS;
    }
  }
  generate_patch(worldMap, MOUNTAINS, 5, 10, 15, 1);
  generate_patch(worldMap, FOREST, 5, 10, 15, 1);

  return worldMap;
}

// Rendering function

// Memory cleanup
void destroyWorldMap(s_WorldMap *worldMap) {
  for (int y = 0; y < worldMap->height; y++) {
    free(worldMap->tiles[y]);
  }
  free(worldMap->tiles);
  free(worldMap);
}
// Initialize the game state
s_GameState *initGameState() {
  if (state == NULL) {
    state = malloc(sizeof(s_GameState));
    // Rest of initialization
  }
  state->assets = malloc(sizeof(s_AssetManager));
  state->assets->grassTextures = malloc(sizeof(Texture2D));
  state->assets->biomeTextures = malloc(sizeof(Texture2D));
  state->worldMap = malloc(sizeof(s_WorldMap));
  state->worldMap->tiles =
      malloc(sizeof(s_WorldTile *) * (SCREEN_HEIGHT / TILE_SIZE));
  for (int i = 0; i < SCREEN_HEIGHT / TILE_SIZE; i++) {
    state->worldMap->tiles[i] =
        malloc(sizeof(s_WorldTile) * (SCREEN_WIDTH / TILE_SIZE));
  }
  state->worldMap = createWorldMap();
  loadAssets();
  if (state->assets->characterFrames[0].id == 0 ||
      state->assets->characterFrames[1].id == 0) {
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
  return state;
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
