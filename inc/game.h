#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MV_SPEED 5 // 1

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

typedef enum {
  GREEN_TREE,
  GREEN_DARK_TREE,
  PINE_TREE,
  GREEN_PINE_TREE,
  LEAF_TREE,
  LEAF_DARK_TREE,
  BLUE_MTN,
  RED_MTN,
  WHITE_MTN,
  SMALL_WHITE_MTN,
  GREEN_HILL,
  YELLOW_HILL,
} e_BiomeTextures;

typedef enum {
  NE_GRASS,
  N_GRASS,
  NW_GRASS,
  SINGLE_N_GRASS,
  E_GRASS,
  M_GRASS,
  W_GRASS,
  SINGLE_M_GRASS,
  SE_GRASS,
  S_GRASS,
  SW_GRASS,
  SINGLE_S_GRASS,
} e_GrassTextures;

typedef enum {
  GRASSLANDS,
  MOUNTAINS,
  FOREST,
} e_BiomeTypes;

enum Direction { DOWN, UP, LEFT, RIGHT };

typedef struct s_assetManager {
  Texture2D characterFrames[2]; // All character animation frames
  Rectangle characterRects[9];  // Source rectangles for characters
  Texture2D *grassTextures;     // Environment textures (grass etc.)
  Rectangle grassRects[12];
  Texture2D *biomeTextures;
  Rectangle biomeRects[12];
} s_AssetManager;

typedef struct {
    e_BiomeTypes type;
} s_WorldTile;

typedef struct {
    s_WorldTile** tiles;  // 2D array of tiles
    int width;
    int height;
} s_WorldMap;

typedef struct s_entity {
  Rectangle entitySrc[2];
  Rectangle entityDest;
  bool isMoving;
} s_Entity;

typedef struct s_gameState {
  s_AssetManager *assets;
  s_Entity *hero;
  s_WorldMap *worldMap;
  Camera2D camera;
  float frameTime;
  int currentFrame;
} s_GameState;

s_GameState *getGameState();

void input();
void drawWorldMap();
#endif
