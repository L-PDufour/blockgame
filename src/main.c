#include "raylib.h"
#include <math.h>
#include <stdio.h>
#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

// Simple game state
typedef struct {
  Vector2 ballPosition;
  Vector2 ballSpeed;
  Color ballColor;
} GameState;

GameState game = {0};

void UpdateDrawFrame(void);

int main(void) {
  // Initialize raylib
  printf("Starting Raylib initialization...\n");
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Test - RayWasm");

  // Initialize game state
  game.ballPosition = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  game.ballSpeed = (Vector2){2.0f, 3.0f};
  game.ballColor = RED;

  printf("Game initialized successfully!\n");

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }
#endif

  // De-Initialization
  CloseWindow();

  return 0;
}

void UpdateDrawFrame(void) {
  // Update
  game.ballPosition.x += game.ballSpeed.x;
  game.ballPosition.y += game.ballSpeed.y;

  // Bounce off edges
  if ((game.ballPosition.x >= (SCREEN_WIDTH - 20)) ||
      (game.ballPosition.x <= 20))
    game.ballSpeed.x *= -1.0f;
  if ((game.ballPosition.y >= (SCREEN_HEIGHT - 20)) ||
      (game.ballPosition.y <= 20))
    game.ballSpeed.y *= -1.0f;

  // Change color over time
  static float colorTime = 0.0f;
  colorTime += 0.02f;
  game.ballColor =
      (Color){(unsigned char)(127 + 127 * sin(colorTime)),
              (unsigned char)(127 + 127 * sin(colorTime + 2.0f)),
              (unsigned char)(127 + 127 * sin(colorTime + 4.0f)), 255};

  // Draw
  BeginDrawing();

  ClearBackground(DARKBLUE);

  // Draw bouncing ball
  DrawCircleV(game.ballPosition, 20, game.ballColor);

  // Draw text
  DrawText("Simple Raylib Test", 10, 10, 20, WHITE);
  DrawText("If you see this, Raylib is working!", 10, 40, 16, LIGHTGRAY);

  // Show FPS
  DrawFPS(10, SCREEN_HEIGHT - 30);

  EndDrawing();
}
