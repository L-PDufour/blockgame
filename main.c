#include "raylib.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

int main(void) {
  // Initialize the window with Wayland support
  const int screenWidth = 800;
  const int screenHeight = 600;

  // Initialize the Raylib window (Wayland support is handled automatically if
  // your Raylib is compiled with it)
  InitWindow(screenWidth, screenHeight, "Raylib Wayland Example");
  GameScreen currentScreen = LOGO;

  int framesCounter = 0;
  // Set the target FPS (frames per second)
  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {

    switch (currentScreen) {
    case LOGO: {
      framesCounter++;
      if (framesCounter > 120) {
        currentScreen = TITLE;
      }
    } break;
    case TITLE: {
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = GAMEPLAY;
      }
    } break;
    case GAMEPLAY:
      {
    // Update
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = ENDING;
      }
    } break;
    case ENDING:
                  {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = TITLE;
                }
            } break;
            default: break;
        }
    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch (currentScreen) {
    case LOGO:
      {
        // TODO: Draw LOGO screen here!
        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
        DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

      } break;
    case TITLE:
      {
        // TODO: Draw TITLE screen here!
        DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

      } break;
    case GAMEPLAY:
      {
        // TODO: Draw GAMEPLAY screen here!
        DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
        DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
        DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

      } break;
    case ENDING:
      {
        // TODO: Draw ENDING screen here!
        DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

      } break;
    default: break;
    }

    EndDrawing();
  }
  // Close window and OpenGL context
  CloseWindow();

  return 0;
}
