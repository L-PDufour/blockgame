#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MV_SPEED 5

#define TILE_SIZE 16

typedef enum {
  CHARACTER_1,
  CHARACTER_2,
  CHARACTER_3,
  CHARACTER_4,
  CHARACTER_5,
  CHARACTER_6,
  CHARACTER_7,
  CHARACTER_8,
  CHARACTER_9,
} e_CharacterType;

typedef struct s_assetManager {
  Texture2D characterFrames[2];    // All character animation frames
  Rectangle characterRects[9];     // Source rectangles for characters
  Texture2D *environmentTextures;  // Environment textures (grass etc.)
} s_AssetManager;

enum Direction {
  DOWN,
  UP,
  LEFT,
  RIGHT
};

typedef struct s_entity {
  Rectangle entitySrc[2];
  Rectangle entityDest;
  bool isMoving;

} s_Entity;

typedef struct s_gameState {
  s_AssetManager *assets;
  s_Entity *hero;
  Camera2D camera;
  float frameTime;
  int currentFrame;

} s_GameState;

s_GameState *getGameState();

void input();

#endif
