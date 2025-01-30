#include "raylib.h"
#include <stdlib.h>
#include <threads.h>

typedef struct textureGame {
  Texture2D grassSprite;
  Texture2D player;
} TextureGame;

typedef struct player {
  Rectangle playerSrc;
  Rectangle playerDest;

} Player;

typedef struct GameState {
    // Game objects
    TextureGame texture;
    Player player;
    Camera2D camera;
    
    // Game settings
    float movementSpeed;
    bool gameIsRunning;
    
    // Window settings
    int screenWidth;
    int screenHeight;
} GameState;

static GameState *state = NULL;


void drawScene() {
  Vector2 origin = (Vector2){ state->player.playerDest.width/2.0f, state->player.playerDest.height/2.0f };
  DrawTexture(state->texture.grassSprite, 100, 50, WHITE);
  DrawTexturePro(state->texture.player,         // Texture2D
                 state->player.playerSrc,     // Rectangle (source)
                 state->player.playerDest,    // Rectangle (dest)
                 origin,               // Vector2 (origin)
                 0.0f,                 // rotation (you were missing this)
                 WHITE);
}

void input() {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
      state->player.playerDest.x += state->movementSpeed;

    }
        
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        state->player.playerDest.x -= state->movementSpeed;
    }
        
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
      state->player.playerDest.y -= state->movementSpeed;
    }
        
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        state->player.playerDest.y += state->movementSpeed;
    }
}

void update() {
   state->gameIsRunning = !WindowShouldClose();
    float smoothSpeed = 0.1f;  // Adjust this value to change smoothing (0.1 = more smooth, 1.0 = no smoothing)
    
    // Calculate target position (center on player)
    Vector2 targetPos = {
        state->player.playerDest.x + (state->player.playerDest.width/2),
        state->player.playerDest.y + (state->player.playerDest.height/2)
    };
    
    state->camera.target.x += (targetPos.x - state->camera.target.x) * smoothSpeed;
    state->camera.target.y += (targetPos.y - state->camera.target.y) * smoothSpeed;

}

void render() {
  BeginDrawing();
  BeginMode2D(state->camera);  // You were missing BeginMode2D
  ClearBackground(RAYWHITE);
  drawScene();
  EndMode2D();
  EndDrawing();
}

// Initialize the game state
GameState* initGameState() {
    if (state == NULL) {
        state = malloc(sizeof(GameState));
        
        // Initialize window
        state->screenWidth = 800;
        state->screenHeight = 600;
        InitWindow(state->screenWidth, state->screenHeight, "Raylib Game");
        
        // Initialize game objects
        state->movementSpeed = 5.0f;
        state->gameIsRunning = true;
        
        // Initialize camera
        state->camera.target = (Vector2){ 0.0f, 0.0f };
        state->camera.offset = (Vector2){ state->screenWidth/2.0f, state->screenHeight/2.0f };
        state->camera.rotation = 0.0f;
        state->camera.zoom = 1.0f;
        
        // Load textures
        state->texture.grassSprite = LoadTexture("res/Tilesets/Grass.png");
        state->texture.player = LoadTexture("res/Characters/Basic_Character_Spritesheet.png");
        
        // Initialize player
        state->player.playerSrc = (Rectangle){ 0.0f, 0.0f, 48.0f, 48.0f };
        state->player.playerDest = (Rectangle){ 200.0f, 200.0f, 100.0f, 100.0f };
    }
    return state;
}

void quit() {
    // Unload textures before closing
    UnloadTexture(state->texture.grassSprite);
    UnloadTexture(state->texture.player);
    
    // Free the state memory
    free(state);
    
    CloseWindow();
}

int main(void) {
  initGameState();
  SetTargetFPS(60);
  while (state->gameIsRunning) // Detect window close button or ESC key
    {
      input();
      update();
      render();
    }
  quit();


  return 0;
}
