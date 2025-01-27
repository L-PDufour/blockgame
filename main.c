#include "raylib.h"

int main(void) {
  // Initialize the window with Wayland support
  const int screenWidth = 800;
  const int screenHeight = 600;

  // Initialize the Raylib window (Wayland support is handled automatically if
  // your Raylib is compiled with it)
  InitWindow(screenWidth, screenHeight, "Raylib Wayland Example");

  // Set the target FPS (frames per second)
  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello, Raylib on Wayland!", 10, 10, 20, DARKGRAY);
    EndDrawing();
  }

  // Close window and OpenGL context
  CloseWindow();

  return 0;
}
