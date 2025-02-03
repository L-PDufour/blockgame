#include "../inc/game.h"

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
                             TILE_SIZE * 4, // Scale the texture
                             TILE_SIZE * 4},
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
  float smoothSpeed = 0.1f; // Adjust this value to change smoothing (0.1 = more
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
  printf("1\n");
  state->assets->characterFrames[0] = LoadTexture("ressources/human0.png");
  if (state->assets->characterFrames[0].id == 0) {
    printf("Failed to load human0.png\n");
    return;
  }
  printf("2\n");
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
}

// Initialize the game state
s_GameState *initGameState() {
  if (state == NULL) {
    state = malloc(sizeof(s_GameState));
    // Rest of initialization
  }
  state->assets = malloc(sizeof(s_AssetManager));

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
  state->camera.zoom = 1.0f;
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
      free(state->assets);
    }
    if (state->hero) {
      free(state->hero);
    }
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
